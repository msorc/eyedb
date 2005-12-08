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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <eyedb/eyedb.h>
#include "base_p.h"
#include "eyedb/internals/ObjectHeader.h"
#include <eyedblib/rpc_lib.h>
#include <eyedbsm/smd.h>
#include "SessionLog.h"
#include <eyedblib/connman.h>
#include "eyedblib/log.h"
#include "eyedb/Log.h"
#include "GetOpt.h"

#include "serv_lib.h"
#include "kernel.h"

//#define RPC_TIMEOUT

namespace eyedb {
  extern void (*garbage_handler)(void);
  extern void init(int &argc, char *argv[], string *sv_port, string *sv_host,
		   bool purgeargv);
  extern void printVersion();
}

#ifdef RPC_TIMEOUT
extern void (*settimeout)(int);
#endif
namespace eyedbsm {
  extern Boolean backend;
}

using namespace eyedb;

static Bool nod;

static void
check(const char *opt, int i, int argc)
{
  if (i >= argc - 1)
    {
      fprintf(stderr, "eyedb options: missing argument after '%s'\n", opt);
      exit(1);
    }
}

static int
usage(const char *prog)
{
  fprintf(stderr, "usage: %s %s\n", prog, eyedb::getSrvOptionsUsage());
  return 1;
}

static int
help()
{
  fprintf(stderr,
	  "\neyedbd options:\n"
	  "-sv_port <ports>               listening ports\n"
	  "-sv_smdport <port>             smd port\n"
	  "-sv_host <hostname>            host name: default is hostname, could be localhost\n"
	  "-sv_access_file <accessfile>   access file [default is etc/access]\n"
	  "-sv_datdir <datdir>            default volume directory\n"
#ifdef RPC_TIMEOUT
	  "-sv_timeout <timeout>          timeout in seconds [default is 7200 sec]\n"
#endif
	  "-sv_passwd_file <passwdfile>   password file\n"
	  "-sv_nod                        no daemon: keep stdin, stdout, stderr opened\n"
	  "-eyedbroot <root>              eyedb root directory\n"
	  "-eyedbconf <conffile>          configuration file\n"
	  "-eyedblog <logfile>            output log file\n"
	  "-eyedbdbm <dbmfile>            default EYEDBDBM database file\n"
	  "-eyedbversion                  displays the eyedb version\n"
	  "-help                          displays this window\n",
	  smd_get_port());

  exit(0);
  return 0;
}

static int
get_opts(int argc, char *argv[],
	 const char **accessfile,
	 const char **datdir,
	 const char **passwdfile, 
	 const char **sesslogdev,
	 int *sessloglevel,
	 int *nofork)
{
  int i;
  const char *s;

  *passwdfile = 0;
  *accessfile = eyedb::getConfigValue("sv_access_file");
  *nofork = 0;
  *sesslogdev = 0;
  *sessloglevel = 0;

  *datdir = 0;

#define USE_GETOPT
#ifdef USE_GETOPT

  //static const std::string sv_host = "sv_host";
  //static const std::string sv_port = "sv_port";
  //static const std::string sv_smdport = "sv_smdport";
    static const std::string access_file_opt = "access_file";
    static const std::string datdir_opt = "datdir";
    static const std::string passwd_file_opt = "passwd_file";
    static const std::string nod_opt = "nod";
    static const std::string sesslogdev_opt = "sesslogdev";
    static const std::string sessloglevel_opt = "sessloglevel";
    static const std::string help_opt = "help";

    Option opts[] = {
      /*
      Option(host, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("host name", "<host>")),
      Option(port, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("listening port(s)", "<port1[[,port2],...]>")),
      Option(smdport, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("listening eyedbsmd port", "<port>")),
      */
      Option(access_file_opt, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("Access file", "<access file>")),
      Option(datdir_opt, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("Default datafile directory", "<datdir>")),
      Option(passwd_file_opt, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("Password file", "<passwd file>")),
      Option(nod_opt, OptionBoolType(),
	     0, OptionDesc("No daemon: does not close fd 0, 1 & 2")),
      Option('h', help_opt, OptionBoolType(),
	     0, OptionDesc("Display this message"))
    };

    GetOpt getopt(argv[0], opts, sizeof(opts)/sizeof(opts[0]));

    if (!getopt.parse(argc, argv)) {
      print_standard_usage(getopt);
      //getopt.usage("\n");
      return 1;
    }

    GetOpt::Map &map = getopt.getMap();

    if (map.find(help_opt) != map.end()) {
      print_standard_help(getopt, vector<string>());
      //getopt.help(cerr, "  ");
      return 1;
    }

    if (map.find(passwd_file_opt) != map.end())
      *passwdfile = strdup(map[passwd_file_opt].value.c_str());

    if (map.find(access_file_opt) != map.end())
      *accessfile = strdup(map[access_file_opt].value.c_str());

    if (map.find(datdir_opt) != map.end())
      *datdir = strdup(map[datdir_opt].value.c_str());

    if (map.find(nod_opt) != map.end())
      nod = True;

    if (map.find(sesslogdev_opt) != map.end())
      *sesslogdev = strdup(map[sesslogdev_opt].value.c_str());

    if (map.find(sessloglevel_opt) != map.end())
      *sessloglevel = ((OptionIntType *)map[sessloglevel_opt].type)->
	getIntValue(map[sessloglevel_opt].value);

#else
  for (i = 1; i < argc; )
    {
      char *s = argv[i];
      if (s[0] == '-')
	{
	  if (!strcmp(s, "-sv_port"))
	    {
	      check("-sv_port", i, argc);
	      for (++i; i < argc; i++)
		if (*argv[i] == '-')
		  break;
   	        else
		  idbport[port_cnt++] = argv[i];
	    }
	  else if (!strcmp(s, "-sv_host"))
	    {
	      check("-sv_host", i, argc);
	      *host = argv[++i];
	      ++i;
	    }
	  else if (!strcmp(s, "-sv_smdport"))
	    {
	      check("-sv_smdport", i, argc);
	      *smdport = argv[++i];
	      ++i;
	    }
	  else if (!strcmp(s, "-sv_passwd_file"))
	    {
	      check("-sv_passwdfile", i, argc);
	      *passwdfile = argv[++i];
	      ++i;
	    }
	  else if (!strcmp(s, "-sv_nod"))
	    {
	      nod = True;
	      ++i;
	    }
	  else if (!strcmp(s, "-sv_datdir"))
	    {
	      check("-sv_datdir", i, argc);
	      *datdir = argv[++i];
	      ++i;
	    }
	  else if (!strcmp(s, "-sv_access_file"))
	    {
	      check("-sv_access_file", i, argc);
	      *accessfile = argv[++i];
	      ++i;
	    }
#ifdef RPC_TIMEOUT
	  else if (!strcmp(s, "-sv_timeout"))
	    {
	      check("-sv_timeout", i, argc);
	      *timeout = atoi(argv[++i]);
	      i++;
	    }
#endif
	  else if (!strcmp(s, "-help"))
	    return help();
	  /* private options */
	  else if (!strcmp(s, "-loglevel"))
	    {
	      check("-loglevel", i, argc);
	      *loglevel = atoi(argv[++i]);
	      i++;
	    }
	  else if (!strcmp(s, "-nofork"))
	    {
	      *nofork = 1;
	      ++i;
	    }
	  else
	    return usage(argv[0]);
	}
      else
	return usage(argv[0]);
    }
#endif

  if (!*datdir)
    {
      if (s = eyedb::getConfigValue("sv_datdir"))
	*datdir = strdup(s);
      else
	{
	  fprintf(stderr, "configuration variable sv_datdir is not set\n");
	  exit(1);
	}
    }

#ifndef USE_GETOPT
  if (!port_cnt)
    {
      if (s = eyedb::getConfigValue("sv_port"))
	idbport[port_cnt++] = strdup(s);
      else
	idbport[port_cnt++] = Connection::DefaultIDBPortValue;
    }
#endif

  return 0;
}

static SessionLog *sesslog;

void rpc_unlink_socket();

static void
_quit_handler(void *xpid, int)
{
  //printf("quit_handler -> %d vs. %d\n", *(int *)xpid, getpid());

  if (*(int *)xpid == getpid())
    {
      sesslog->remove();
      rpc_unlink_socket();
    }
}

static int
notice(int status)
{
  const char *s;
  if (s = getenv("EYEDBPFD"))
    {
      int pfd = atoi(s);
      if (pfd > 0)
	{
	  write(pfd, &status, sizeof(status));
	  close(pfd);
	}
    }

  return status;
}

int
main(int argc, char *argv[])
{
  rpc_Server *server;
  rpc_PortHandle *port[32];
  int nports;
  const char *idbsvport[32];
  eyedbsm::Status status;
  const char *hostname;
  const char *accessfile, *datdir, *passwdfile;
  const char *sesslogdev;
  int sessloglevel;

  int nofork;

  string sv_host, sv_port;

  eyedb::init(argc, argv, &sv_host, &sv_port, true);

  nports = 0;

  if (sv_port.length()) {
    // TBD : should parse comma separated port
    idbsvport[nports++] = sv_port.c_str();
  }
  else {
    const char *s;
    if (s = eyedb::getConfigValue("sv_port"))
      idbsvport[nports++] = strdup(s);
    else
      idbsvport[nports++] = Connection::DefaultIDBPortValue;
  }

  hostname = sv_host.c_str();

  server = rpcBeInit();

  garbage_handler = GARBAGE;
#ifdef RPC_TIMEOUT
  settimeout = rpc_setTimeOut;
#endif

  eyedbsm::backend = eyedbsm::True;

  if (get_opts(argc, argv, &accessfile, &datdir, &passwdfile, &sesslogdev,
	       &sessloglevel, &nofork))
    return notice(1);

  /*
  if (smdport)
    smd_set_port(smdport);
  */

  rpc_setProgName(argv[0]);

  if (rpc_connman_init(accessfile))
    return notice(1);

  if (!nports)
    {
      fprintf(stderr, "eyedbd: at least one sv_port must be set.\n");
      return notice(1);
    }

  for (int i = 0; i < nports; i++)
    if (rpc_portOpen(server, hostname, idbsvport[i], &port[i]) != rpc_Success)
      return notice(1);

  int *pid = new int;
  *pid = getpid();
  rpc_setQuitHandler(_quit_handler, pid);

  const char *sv_tmpdir = eyedb::getConfigValue("sv_tmpdir");

  if (!sv_tmpdir)
    sv_tmpdir = "/tmp";

  std::string logdir = sv_tmpdir;

  sesslog = new SessionLog(logdir.c_str(), eyedb::getVersion(),
			      nports, idbsvport,
			      datdir, sesslogdev, sessloglevel);

  IDB_init(datdir, passwdfile, sesslog, 0 /*timeout*/);

  if (sesslog->getStatus())
    {
      sesslog->getStatus()->print();
      return notice(1);
    }

  (void)notice(0);

  if (!nod)
    {
      close(0);
      if (!Log::getLog() || strcmp(Log::getLog(), "stdout"))
	close(1);
      if (!Log::getLog() || strcmp(Log::getLog(), "stderr"))
	close(2);
    }

  rpc_serverMainLoop(server, port, nports);
  return 0;
}
