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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#if defined(SOLARIS) || defined(ULTRASOL7)
#include <stropts.h>
#endif
#include <sys/uio.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <assert.h>
#include <errno.h>
#include <sys/select.h>
#include <time.h>
#include <eyedblib/connman.h>
#include <eyedblib/log.h>


#if defined(SOLARIS) || defined(ULTRASOL7)
#define BYTE1(addr) (addr->_S_un._S_un_b.s_b1)
#define BYTE2(addr) (addr->_S_un._S_un_b.s_b2)
#define BYTE3(addr) (addr->_S_un._S_un_b.s_b3)
#define BYTE4(addr) (addr->_S_un._S_un_b.s_b4)
#else
#define BYTE1(addr) (addr->s_addr >> 24)
#define BYTE2(addr) ((addr->s_addr >> 16) & 0xff)
#define BYTE3(addr) ((addr->s_addr >> 8) & 0xff)
#define BYTE4(addr) (addr->s_addr & 0xff)
#endif

static void
print_addr(FILE *fd, struct in_addr *addr)
{
  if (!BYTE1(addr))
    fprintf(fd, "+");
  else
    fprintf(fd, "%d.%d.%d.%d", BYTE1(addr), BYTE2(addr), BYTE3(addr), BYTE4(addr));
}

static int
cmp_addr(const struct in_addr *a1, const struct in_addr *a2)
{
  if (BYTE1(a1) && BYTE1(a1) != BYTE1(a2))
    return 0;

  if (BYTE2(a1) && BYTE2(a1) != BYTE2(a2))
    return 0;
  
  if (BYTE3(a1) && BYTE3(a1) != BYTE3(a2))
    return 0;
  
  if (BYTE4(a1) && BYTE4(a1) != BYTE4(a2))
    return 0;

  return 1;
}

static int
hostname2addr(const char *name, struct in_addr *addr)

{
  struct hostent *hp;

  memset(addr, 0, sizeof(*addr));

  if (!(hp = gethostbyname(name)))
    return 1;

  memcpy((char *)addr, (char *)hp->h_addr, hp->h_length);

  return 0;
}

static const char *rpc_access_file;

typedef struct {
  struct in_addr addr;
  rpc_TcpIp tcpip;
} rpc_Access;

#define TAKE_CHAR(S) \
 *p++ = 0; \
 words[nw++] = S; \
 empt = 1

int
line_parse(FILE *fd, char buf[], char **words, int *line)
{
  for (;;) {
    char *p;
    char c;
    int empt = 1;
    int nw = 0;
    int isquote = 0;

    if (!fgets(buf, 256, fd))
      return -1;

    (*line)++;
    p = buf;

    while ((c = *p) == ' ' || c == '\t')
      p++;

    while (c = *p) {
      switch(c) {
      case '#':
	if (!empt)
	  nw++;
	*p++ = 0;
	while (c = *++p)
	  ;
	break;

      case '=':
	TAKE_CHAR("=");
	break;

      case '!':
	TAKE_CHAR("!");
	break;

      case '+':
	TAKE_CHAR("+");
	break;

      case ' ':
      case '\t':
      case '\n':
	empt = 1;
	*p++ = 0;
	break;

      default:
	if (empt) {
	  empt = 0;
	  words[nw++] = p;
	}

	p++;
	break;
      }
    }

    *p = 0;
    words[nw] = 0;
    return nw;
  }
}

static int access_cnt;
static rpc_Access rpc_access[512];

static void
make_user(char *words[], int *n, rpc_User *user)
{
  char *s = words[(*n)++];

  if (!strcmp(s, "!"))
    user->mode = rpc_User::NOT;
  else if (!strcmp(s, "="))
    user->mode = rpc_User::DEF;
  else if (!strcmp(s, "+")) {
    user->mode = rpc_User::ALL;
    user->user = strdup("");
    return;
  }
  else {
    user->mode = rpc_User::ON;
    user->user = strdup(s);
    return;
  }

  user->user = strdup(words[(*n)++]);
  return;
}

static const char *
get_str(int mode)
{
  if (mode == rpc_User::ON)
    return "";
  if (mode == rpc_User::NOT)
    return "not ";
  if (mode == rpc_User::DEF)
    return "default=";
  if (mode == rpc_User::ALL)
    return "+";
  return "<unknown>";
}

static void
free_access()
{
  int i, j;
  for (i = 0; i < access_cnt; i++) {
    for (j = 0; j < rpc_access[i].tcpip.user_cnt; j++)
      free(rpc_access[i].tcpip.users[j].user);

    free(rpc_access[i].tcpip.users);
  }
  access_cnt = 0;
}

void
rpc_print_tcpip(FILE *fd, rpc_TcpIp *ci)
{
  int j;
  for (j = 0; j < ci->user_cnt; j++)
    fprintf(fd, "%s%s%s", (j ? " " : ""),  get_str(ci->users[j].mode),
	    ci->users[j].user);

  fprintf(fd, "\n");
}

static int
read_access_file_realize()
{
  FILE *fd;
  char *words[32];
  int nw, i, j, u;
  char buf[256];
  int line = 0;

  if (!(fd = fopen(rpc_access_file, "r"))) {
    fprintf(stderr, "cannot open access file '%s' for reading\n",
	    rpc_access_file);
    return 1;
  }

  free_access();

  while ((nw = line_parse(fd, buf, words, &line)) >= 0) {
    if (!nw)
      continue;

    if (nw < 2) {
      fprintf(stderr, "access file %s: syntax error at line #%d\n",
	      rpc_access_file, line);
      continue;
    }

    if (!strcmp(words[0], "+"))
      memset(&rpc_access[access_cnt].addr, 0, sizeof(struct in_addr));
    else if (hostname2addr(words[0], &rpc_access[access_cnt].addr)) {
      fprintf(stderr, "access file %s: invalid host name at line #%d: "
	      "%s\n", rpc_access_file, line, words[0]);
      continue;
    }

    /* 2/09/05: already freed in free_access() */
    /*
    for (j = 0; j < rpc_access[access_cnt].tcpip.user_cnt; j++)
      free(rpc_access[access_cnt].tcpip.users[j].user);
    free(rpc_access[access_cnt].tcpip.users);
    */

    rpc_access[access_cnt].tcpip.users = (rpc_User *)calloc((nw-1)*sizeof(rpc_User), 1);
    for (j = 1, u = 0; j < nw; u++)
      make_user(words, &j, &rpc_access[access_cnt].tcpip.users[u]);

    rpc_access[access_cnt++].tcpip.user_cnt = u;
  }

  fclose(fd);

  /*
  for (i = 0; i < access_cnt; i++) {
    print_addr(stderr, &rpc_access[i].addr);
    fprintf(stderr, ": ");
    rpc_print_tcpip(stderr, &rpc_access[i].tcpip);
  }
  */

  return 0;
}

static int
read_access_file()
{
  static time_t last_read;
  struct stat st;
  int r;

  if (!rpc_access_file)
    return 0;

  if (stat(rpc_access_file, &st) < 0) {
    fprintf(stderr, "cannot stat access file '%s'\n", rpc_access_file);
    return 1;
  }

  if (st.st_mtime <= last_read)
    return 0;

  r = read_access_file_realize();
  time(&last_read);
  return r;
}

int
rpc_connman_init(const char *access_file)
{
  rpc_access_file = access_file;
  return read_access_file();
}

rpc_ConnInfo *
rpc_check_addr(struct in_addr *addr)
{
  int i;
  if (read_access_file())
    return 0;

  for (i = 0; i < access_cnt; i++) {
    if (cmp_addr(&rpc_access[i].addr, addr)) {
      rpc_ConnInfo *ci = (rpc_ConnInfo *)calloc(sizeof(rpc_ConnInfo), 1);
#ifdef STUART_AUTH
      ci->tcpip = rpc_access[i].tcpip;
#else
      ci->u.tcpip = rpc_access[i].tcpip;
#endif
      return ci;
    }
  }

  IDB_LOG(IDB_LOG_CONN,
	  ("connection refused to %d.%d.%d.%d\n", BYTE1(addr), BYTE2(addr), BYTE3(addr), BYTE4(addr)));

  return 0;
}

rpc_ConnInfo *
rpc_make_tcpip_conninfo(int fd)
{
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);

  memset(&addr, 0, sizeof(addr));

  if (getpeername(fd, (struct sockaddr *)&addr, &len)) {
    perror("getpeername");
    return (rpc_ConnInfo *)0;
  }

  rpc_ConnInfo *ci = rpc_check_addr(&addr.sin_addr);
  if (ci)
    ci->mode = rpc_ConnInfo::TCPIP;
  return ci;
}

static rpc_ConnInfo *
rpc_check_localhost()
{
  struct in_addr addr;

  if (hostname2addr("localhost", &addr))
    return 0;

  return rpc_check_addr(&addr);
}

#ifdef IDB_STREAM
rpc_ConnInfo *
rpc_make_stream_conninfo(int fd, struct strrecvfd *info)
{
  rpc_ConnInfo *ci = rpc_check_localhost();

  if (ci) {
    ci->mode = rpc_ConnInfo::STREAM;
#ifdef STUART_AUTH
    ci->auth.uid = info->uid;
    ci->auth.gid = info->gid;
#else
    ci->u.stream.uid = info->uid;
    ci->u.stream.gid = info->gid;
#endif
  }

  return ci;
}

#else

rpc_ConnInfo *
rpc_make_unix_conninfo(int fd)
{
  rpc_ConnInfo *ci = rpc_check_localhost();

  if (ci)
    ci->mode = rpc_ConnInfo::UNIX;

  return ci;
}

#endif

#if 0
int
main(int argc, char *argv[])
{
  rpc_connman_init(argv[1]);
  read_access_file_realize();
  read_access_file_realize();
  return 0;
}
#endif
