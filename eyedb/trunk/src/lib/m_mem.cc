/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
  Author: Eric Viara <viara@sysra.com>
*/

#include <eyedbconfig.h>

#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <stdarg.h>

#include <eyedblib/m_mem.h>
#include <eyedblib/log.h>
#include <eyedblib/iassert.h>
#include <lib/m_mem_p.h>

struct m_Map {
  caddr_t *p;
  caddr_t addr;
  size_t size;
  int prot, flags, fildes;
  off_t off;
  u_int ref;
  char locked;
  void (*gtrig)(void *client_data);
  void *client_data;
  char *file;
  int startns, endns;
  m_Map *prev, *next;
};

static pthread_mutex_t m_mp;
static int init_done;
static m_Map *m_head;
static size_t m_tmsize, m_maxsize;

static const u_int ONE_K = 1024;
static const size_t m_min_maxsize = ONE_K * ONE_K * ONE_K; // 1 Gb
static u_int  m_ref = 0x100,;

static int m_getunlocked()
{
  int cnt = 0;
  m_Map *m;

  m = m_head;

  while (m) {
    if (!m->locked)
      cnt++;
    m = m->next;
  }

  return cnt;
}

/*#define TRACE*/

static int m_garbage(m_Map *rm)
{
  m_Map *km = rm;

  pthread_mutex_lock(&m_mp);

  if (!rm) {
    u_int ref = 0xffffffff;
    m_Map *m;
    m = m_head;

    while(m) {
      if (m->ref < ref && !m->locked) {
	ref = m->ref;
	rm = m;
      }
      m = m->next;
    }

  }

  if (rm) {
    m_tmsize -= rm->size;

    if (rm->prev)
      rm->prev->next = rm->next;
    if (rm->next)
      rm->next->prev = rm->prev;

    if (rm == m_head)
      m_head = rm->next;

    pthread_mutex_unlock(&m_mp);

    if (!km && rm->gtrig)
      rm->gtrig(rm->client_data);

    IDB_LOG(IDB_LOG_MMAP_DETAIL,
	    ("m_garbage: unmapping %p for size %u\n", *rm->p, rm->size));

#ifdef CYGWIN
    printf("m_garbage: unmapping %p for size %u\n", *rm->p, rm->size);
#endif

    if (munmap(*rm->p, rm->size)) {
      utlog("munmap(%p, %d)\n", *rm->p, rm->size);
      //perror("munmap")
      abort();
    }

    *rm->p = 0;

    // 4/10/05
    free(rm->file);
    free(rm);

    /*
    if (km) { //free only when m_munmap()
      free(rm->file);
      free(rm);
    }
    */

    return 0;
  }

  pthread_mutex_unlock(&m_mp);
  IDB_LOG(IDB_LOG_MMAP_DETAIL, ("m_garbage failed!\n"));

  return 1;
}

static void m_insert(m_Map *m)
{
  pthread_mutex_lock(&m_mp);

  m->next = m_head;
  m->prev = 0;

  if (m_head)
    m_head->prev = m;
  
  m->ref = ++m_ref;
  m_head = m;
  
  m_tmsize += m->size;

  pthread_mutex_unlock(&m_mp);
}

/* guess that this routine is called soon enough in the program
   so that the thought stack address (&r) is correct;
   in the eyedbsm package, this routine is called from se_init() */

/*#define USE_MADVISE*/

#ifdef USE_MADVISE
static int must_madvise;
#endif

void m_init(void)
{
  pthread_mutexattr_t mattr;

  if (init_done)
    return;

  pthread_mutexattr_init(&mattr);

#ifdef HAVE_PTHREAD_MUTEXATTR_SETPSHARED
  pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
#endif

  pthread_mutex_init(&m_mp, &mattr);

#ifdef USE_MADVISE
  must_madvise = getenv("EYEDB_MADVISE") ? 1 : 0;
#endif

  init_done = 1;
}

static void reduce_memory(size_t size)
{
  if (!m_maxsize)
    return;

  bool must_reduced = false;
  while (size + m_tmsize > m_maxsize) {
    must_reduced = true;
    IDB_LOG(IDB_LOG_MMAP_DETAIL,
	    ("must reduced total size: %llu\n", m_tmsize));
    if (m_garbage(0))
      break;
  }
  if (must_reduced)
    IDB_LOG(IDB_LOG_MMAP_DETAIL, ("reduced: %llu\n", m_tmsize));
}

void m_gtrig_set(m_Map *m, void (*gtrig)(void *client_data), void *client_data)
{
  m->gtrig = gtrig;
  m->client_data = client_data;
}

m_Map *m_mmap(caddr_t addr, size_t size, int prot, int flags,
	      int fildes, off_t off, caddr_t *p, const char *file,
	      off_t startns, off_t endns)
{
  if (!init_done)
    m_init();

  IDB_LOG(IDB_LOG_MMAP_DETAIL,
	  ("mapping attempt file=\"%s\" size=%lu prot=%p "
	   "flags=%p fd=%d offset=%u",
	   file, size, prot, flags, fildes, off));

#ifdef CYGWIN
  printf("... mapping attempt file=\"%s\" size=%lu prot=%p "
	 "flags=%p fd=%d offset=%u\n",
	 file, size, prot, flags, fildes, off);
#endif

  errno = 0;

  if (endns > 0)
    IDB_LOG_X(IDB_LOG_MMAP_DETAIL, (" startns=%d endns=%d\n", startns, endns));
  else
    IDB_LOG_X(IDB_LOG_MMAP_DETAIL, ("\n"));

  reduce_memory(size);

  /* disconnected the 25/07/01 */
#if 0
  /* re-added the 8/12/99 */
  while (!is_enough_place(size))
    if (m_garbage(0))
      break; /* was: return 0; */
  /* ... */
#endif

  for (unsigned int ntries = 0;; ntries++) {
    if ((*p = (caddr_t)mmap(addr, size, prot, flags, fildes, off)) != MAP_FAILED) {
      m_Map *m = (m_Map *)calloc(sizeof(m_Map), 1);
	  
      assert(m);
      m->p = p;
      m->addr = addr;
      m->size = size;
      m->prot = prot;
      m->flags = flags;
      m->fildes = fildes;
      m->off = off;
      m->file = strdup(file);
      m->startns = startns;
      m->endns = endns;

      m_insert(m);

      IDB_LOG(IDB_LOG_MMAP,
	      ("segment mapped file=\"%s\" segment=[%p, %p[ "
	       "size=%lu prot=%p flags=%p fd=%d offset=%u",
	       file, *p, (*p)+size, size, prot, flags, fildes, off));

#ifdef CYGWIN
      printf("segment mapped file=\"%s\" segment=[%p, %p[ "
	     "size=%lu prot=%p flags=%p fd=%d offset=%u\n",
	     file, *p, (*p)+size, size, prot, flags, fildes, off);

      printf("*%p = %d\n", *p, **p);
      if (size > 0x400)
	printf("--> *%p = %d\n", (*p)+0x400, *((*p)+0x400));
#endif

      if (endns > 0)
	IDB_LOG_X(IDB_LOG_MMAP,
		  (" startns=%d endns=%d\n", startns, endns));
      else
	IDB_LOG_X(IDB_LOG_MMAP, ("\n"));

#ifdef USE_MADVISE
      if (must_madvise && madvise(*p, size, MADV_RANDOM)) {
	IDB_LOG(IDB_LOG_MMAP, 
		("madvise failed for %p %d\n", *p, size));
	perror("madvise");
      }
#endif
      return m;
    }

    IDB_LOG(IDB_LOG_MMAP_DETAIL,
	    ("mapping failed file=\"%s\" size=%lu prot=%p flags=%p "
	     "fd=%d offset=%u attempt=#%d\n",
	     file, size, prot, flags, fildes, off, ntries));

    //perror("m_mem.c: mmap");

#ifdef CYGWIN
    printf("mapping failed file=\"%s\" size=%lu prot=%p flags=%p "
	   "fd=%d offset=%u attempt=#%d\n",
	   file, size, prot, flags, fildes, off, ntries);
#endif

    if (endns > 0)
      IDB_LOG_X(IDB_LOG_MMAP_DETAIL,
		(" startns=%d endns=%d\n", startns, endns));
    else
      IDB_LOG_X(IDB_LOG_MMAP, ("\n"));
    
    if (m_garbage(0))
      break;
  }

  *p = 0;

  IDB_LOG((eyedblib::LogMask)~0,
	  ("mapping *definitively* failed file=\"%s\""
	   "size=%lu prot=%p flags=%p fd=%d offset=%u startns=%d endns=%d\n",
	   file, *p, (*p)+size, size, prot, flags, fildes, off, startns, endns));

  /*perror("mmap");
    printf("m_mmap(size=%lu) failed after %d attemps\n", size, ntries);*/

  /*abort(); */ /* added the 23/08/99 */
  return 0;
}

void m_lock(m_Map *m)
{
  m->locked = 1;
}

void m_unlock(m_Map *m)
{
  m->locked = 0;
}

void m_access(m_Map *m)
{
  m->ref = ++m_ref;
}

int m_munmap(m_Map *map, caddr_t addr, size_t size)
{
  IDB_LOG(IDB_LOG_MMAP,
	  ("segment unmapped file=\"%s\" segment=[%p, %p[ "
	   "size=%lu prot=%p flags=%p fd=%d offset=%u "
	   "startns=%d endns=%d\n",
	   map->file, *map->p, (*map->p)+map->size, map->size, map->prot,
	   map->flags, map->fildes, map->off,
	   map->startns, map->endns));

  //  assert(map->size == size);

  if (map->size != size) {
    IDB_LOG(IDB_LOG_MMAP, 
	    ("warning unmap size differ : %lu %lu",
	     map->size, size));
  }


#ifdef CYGWIN
  printf("segment unmapped file=\"%s\" segment=[%p, %p[ "
	 "size=%lu prot=%p flags=%p fd=%d offset=%u "
	 "startns=%d endns=%d\n",
	 map->file, *map->p, (*map->p)+map->size, map->size, map->prot,
	 map->flags, map->fildes, map->off,
	 map->startns, map->endns);
#endif

  return m_garbage(map);
}

void *m_malloc(size_t size)
{
  void *p;

  if (!init_done)
    m_init();

  if (!size)
    size = 4;

  while (!(p = malloc(size)))
    if (errno != ENOMEM || m_garbage(0)) /* really ? */
      m_abort_msg("malloc(%lu) failed [errno %d]\n", size, errno);

  return p;
}

void *m_calloc(size_t nelem, size_t elsize)
{
  void *p;

  if (!init_done)
    m_init();

  while (!(p = calloc(nelem, elsize)))
    if (errno != ENOMEM || m_garbage(0))
      m_abort_msg("calloc(%d, %lu) failed [errno %d]\n", nelem, elsize, 
		  errno);
	   
  return p;
}

void *m_realloc(void *ptr, size_t size)
{
  void *p;

  if (!init_done)
    m_init();

  while (!(p = realloc(ptr, size)))
    if (errno != ENOMEM || m_garbage(0))
      m_abort_msg("realloc(%p, %lu) failed [errno %d]\n", ptr, size,
		  errno);
	   
  return p;
}

void m_free(void *ptr)
{
  free(ptr);
}

void m_abort_msg(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  m_maptrace(std::cerr);
  vfprintf(stderr, fmt, ap);
  m_abort();
  va_end(ap);
}

void m_abort()
{
  m_mmaps_garbage();
  abort();
}

void m_mmaps_garbage()
{
  m_Map *m = m_head, *next;

  while (m) {
    next = m->next;
    m_garbage(m);
    m = next;
  }
}
 
void m_maptrace(std::ostream &os)
{
  m_Map *m = m_head;

  os << "----------------------- eyedb memory map manager ---------------------\n";

  while (m) {
    os << " addr " << *m->p << " size " << m->size << "[" <<
      (m->size/ONE_K) << " kb\n";
    m = m->next;
  }

  os << " total memory used: " << (m_tmsize/ONE_K) << " kb\n";
  if (m_maxsize)
    os << " maximum memory size: " << (m_maxsize/ONE_K) << " kb\n";
    
}

size_t m_get_totalsize()
{
  return m_tmsize;
}

void m_set_maxsize(size_t maxsize)
{
  m_maxsize = maxsize;
  if (m_maxsize != 0 && m_maxsize < m_min_maxsize)
    m_maxsize = m_min_maxsize;

  reduce_memory(0);
}

size_t m_get_maxsize()
{
  return m_maxsize;
}
