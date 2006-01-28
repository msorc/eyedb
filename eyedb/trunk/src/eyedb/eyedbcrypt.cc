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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
  char s[64];

  if (argc != 3)
    {
      fprintf(stderr, "usage: %s cryptfile passwd\n", argv[0]);
      return 1;
    }

  int fd = creat(argv[1], 0600);

  if (fd < 0)
    {
      fprintf(stderr, "%s: cannot create file '%s'\n", argv[0], argv[1]);
      return 1;
    }
    
  if (strlen(argv[2]) > 8)
    argv[2][8] = 0;

  strcpy(s, crypt(argv[2], "r8"));
  s[13] = 0;

  if (write(fd, s, 14) != 14)
    {
      fprintf(stderr, "%s: cannot write file '%s'\n", argv[0], argv[1]);
      return 1;
    }

  fchmod(fd, 0400);

  close(fd);

  return 0;
}
