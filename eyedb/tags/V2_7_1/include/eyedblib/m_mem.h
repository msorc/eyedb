
/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
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


#ifndef _EYEDBLIB_M_MEM_H
#define _EYEDBLIB_M_MEM_H

#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct m_Map m_Map;

extern m_Map
  *m_mmap(caddr_t addr, size_t len, int prot, int flags,
	  int fildes, off_t off, caddr_t *p, const char *file,
	  off_t startns, off_t endns),
  *m_remap(m_Map *m);

extern int
  m_munmap(m_Map *map, caddr_t addr, size_t len);

extern u_int
  m_data_margin_set(u_int data_margin);

extern void
  m_init(void),
  m_access(m_Map *map),
  m_lock(m_Map *m),
  m_unlock(m_Map *m),
  m_gtrig_set(m_Map *m, void (*gtrig)(void *client_data), void *client_data),
  
  *m_malloc(size_t len),
  *m_calloc(size_t nelem, size_t elsize),
  *m_realloc(void *ptr, size_t size),
  m_free(void *ptr),

  m_abort(void),
  m_abort_msg(const char *fmt, ...),
  m_mmaps_garbage(void),
  m_maptrace(FILE *fd);

#endif
