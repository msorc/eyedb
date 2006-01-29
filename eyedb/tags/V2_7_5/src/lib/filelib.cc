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
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <eyedblib/filelib.h>


/*
14/02/05: was :
//@@@@ Id rather discard solaris specific, non-standard code
#if defined(LINUX) || defined(LINUX64) || defined(LINUX_IA64) || defined(LINUX_PPC64) || defined(ORIGIN) || defined(ALPHA) || defined(AIX)  || defined(CYGWIN)
instead of HAS_FLOCK_T
*/

#ifdef HAS_FLOCK_T
static flock_t flock;
#else
static struct flock flock;
#endif

int ut_file_lock(int fd, ut_Lock excl, ut_Block block)
{
  flock.l_type = (excl == ut_LOCKX ? F_WRLCK : F_RDLCK);
  return fcntl(fd, (block == ut_BLOCK ? F_SETLKW : F_SETLK), &flock);
}

int ut_file_unlock(int fd)
{
  flock.l_type = F_UNLCK;
  return fcntl(fd, F_SETLK, &flock);
}
