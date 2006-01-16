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

#include "eyedbconfig.h"

#include <eyedblib/butils.h>
#include <eyedblib/thread.h>
#include <stdlib.h>

#define IS_INT(c) (c == 'D' || c == 'I' || c == 'O' || c == 'X')

#define NBUFS 8

namespace eyedblib {
  static Mutex mt;
}

namespace eyedblib {
  char *
  getFBuffer(const char *fmt, va_list ap)
  {
    static int argstr_alloc;
    static enum Type {INT, LONG, STR} *argstr;
    static int buffer_which;
    static int buffer_len[NBUFS];
    static char *buffer[NBUFS];

    MutexLocker _(mt);
    int which, n;
    int argstr_cnt = 0;
    int argint_cnt = 0;
    int arg_cnt = 0;
    unsigned int len = 0;

    unsigned int len_1 = strlen(fmt);
    for (; ;fmt++)
      {
	char c = *fmt;
	char num[32];
	char *pnum;
	int is_long;

	if (!c)
	  break;

	if (c != '%')
	  {
	    len++;
	    continue;
	  }

	is_long = 0;
	pnum = num;
	while (c = *++fmt)
	  {
	    if (!c)
	      break;
	  
	    if (c >= '0' && c <= '9')
	      {
		*pnum++ = c;
		continue;
	      }

	    if (c == 'l')
	      {
		is_long++;
		continue;
	      }

	    c = (c >= 'a' && c <= 'z' ? c + 'A'-'a' : c);

	    if (IS_INT(c) || c == 'U' || c == 'E' || c == 'G' || c == 'C')
	      {
		if (c == 'U' && (IS_INT(*(fmt+1)) || *(fmt+1) == 'l'))
		  continue;

		if (arg_cnt >= argstr_alloc)
		  {
		    argstr_alloc = arg_cnt + 12;
		    argstr = (enum Type *)
		      realloc(argstr, sizeof(enum Type)*argstr_alloc);
		  }

		if (is_long > 1)
		  {
		    argstr[arg_cnt++] = LONG;
		    argint_cnt++;
		  }
		else
		  argstr[arg_cnt++] = INT;

		argint_cnt++;
		break;
	      }

	    if (c == 'S')
	      {
		if (arg_cnt >= argstr_alloc)
		  {
		    argstr_alloc = arg_cnt + 12;
		    argstr = (enum Type *)
		      realloc(argstr, sizeof(enum Type)*argstr_alloc);
		  }

		argstr[arg_cnt++] = STR;
		argstr_cnt++;
		break;
	      }
	  
	    if (c == '%')
	      {
		len++;
		break;
	      }

	    /* and what about float and double!!!!!! */
	    /* len += 512; */

	    /* unknown format sequence !? */
	    len += 2;
	    break;
	  }

	if (pnum != num)
	  {
	    *pnum = 0;
	    len += atoi(num);
	  }
      }

    len += argint_cnt * 20;

    for (n = 0; n < arg_cnt; n++)
      {
	if (argstr[n] == STR)
	  len += strlen(va_arg(ap, char *));
	else if (argstr[n] == LONG)
	  (void)va_arg(ap, long long);
	else
	  (void)va_arg(ap, int);
      }

    which = buffer_which;
    if (which >= NBUFS)
      which = 0;

    if (len+1 >= buffer_len[which])
      {
	free(buffer[which]);
	buffer_len[which] = len + 128;
	buffer[which] = (char *)malloc(buffer_len[which]);
      }

    return buffer[which++];
  }

  /*@@@@*/
#if defined(LINUX) || defined(LINUX64) || defined(LINUX_IA64) || defined(LINUX_PPC64) || defined(ORIGIN) || defined(ALPHA) || defined(CYGWIN)
#include <time.h>
#include <stdio.h>

  void display_time(const char *fmt, ...)
  {
    va_list ap;
    struct timeval tp;
    unsigned long long ms;

    gettimeofday(&tp, NULL);

    ms = (unsigned long long)tp.tv_sec * 1000 + tp.tv_usec/1000;

    va_start(ap, fmt);

    vfprintf(stdout, fmt, ap);
    fprintf(stdout, ": %lld ms\n", ms);
    va_end(ap);
  }
#endif

  int
  is_number(const char *s)
  {
    char c;
    if (!*s)
      return 0;
  
    while (c = *s++)
      if (!(c >= '0' && c <= '9'))
	return 0;

    return 1;
  }

}
