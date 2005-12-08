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


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <eyedb/eyedb.h>
#include "SessionLog.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <eyedbsm/smd.h>
#include <eyedblib/strutils.h>
#include "comp_time.h"

using namespace eyedb;

namespace eyedb {
  extern void init(int &argc, char *argv[], string *sv_port, string *sv_host,
		   bool purgeargv);
}

static int
usage(const char *prog)
{
#if 1
  cerr << "usage: " << prog << " ";
  print_common_usage(cerr);
  cerr << " start|stop|status [-f] [-h|--help]\n";
#else
  static const char etc[] = "{eyedbd options}";
  fprintf(stderr, "usage: %s start [-f] %s\n", prog, etc);
  fprintf(stderr, "       %s stop [-f] %s\n", prog, etc);
  fprintf(stderr, "       %s status %s\n", prog, etc);
  fprintf(stderr, "\nwhere %s is as follows:\n%s\n", etc, eyedb::getSrvOptionsUsage());
#endif

  return 1;
}

static void help(const char *prog)
{
  usage(prog);
  cerr << "\nProgram Options:\n";
  cerr << "  start [-f] Launch the server. Set -f to force launch\n";
  cerr << "  stop [-f]  Stop the server. Set -f to force stop\n";
  cerr << "  status     Display status on the running server\n";
  cerr << "\nCommon Options:\n";
  print_common_help(cerr);
}

static void
check(const char *opt, int i, int argc)
{
  if (i >= argc - 1) {
    fprintf(stderr, "eyedb options: missing argument after '%s'\n", opt);
    exit(1);
  }
}

enum Command {
  Start = 1,
  Stop,
  CStatus
};

static void
ld_libpath_manage()
{
  static const char ld_libpath_env[] = "LD_LIBRARY_PATH";
  char *ld_library_path = getenv(ld_libpath_env);
  char env[2048];
  sprintf(env, "%s=%s%s%s", ld_libpath_env,
	  eyedb::getConfigValue("sopath"),
	  (ld_library_path ? ":" : ""),
	  (ld_library_path ? ld_library_path : ""));
  putenv(strdup(env));
}

static const char *eyedbroot;
#define PIPE_SYNC

extern char **environ;

static int
execute(const char *prog, const char *arg, Bool pipes)
{
  int pid;

#ifdef PIPE_SYNC
  int pfd[2];
  if (pipes && pipe(pfd) < 0) {
    fprintf(stderr, "eyedbrc: cannot create unnamed pipes\n");
    return 1;
  }
#endif

  if ((pid = fork()) == 0) {
    std::string cmd = std::string(eyedbroot) + "/bin/" + prog;
    char *argv[3];
    argv[0] = (char *)prog;
    argv[1] = (char *)arg;
    argv[2] = 0;
#ifdef PIPE_SYNC
    if (pipes) {
      close(pfd[0]);
      putenv(strdup((std::string("EYEDBPFD=") + str_convert((long)pfd[1])).c_str()));
    }
#endif
    if (execve(cmd.c_str(), argv, environ) < 0) {
      fprintf(stderr, "eyedbrc: cannot execute '%s'\n", cmd.c_str());
      return -1;
    }
  }

#ifdef PIPE_SYNC
  if (pipes) {
    int status = 0;
    if (read(pfd[0], &status, sizeof(status)) != sizeof(status)) {
      fprintf(stderr, "eyedbrc: eyedbd smd start failed\n");
      return 0;
    }
      
    if (status)
      return -1;
  }
#endif
  return pid;
}

static int
startServer(int argc, char *argv[], const char *smdport)
{
  if (!eyedbroot) {
    fprintf(stderr, "The configuration variable root or the -eyedbroot command line option must be set.\n");
    return 1;
  }

  smdcli_conn_t *conn = smdcli_open(smd_get_port());
  if (conn) {
    if (smdcli_declare(conn) < 0)
      return 1;
    smdcli_close(conn);
    conn = 0;
  }
  else if (execute("eyedbsmd", (std::string("--port=") + smdport).c_str(), True) < 0)
    return 1;

#ifdef PIPE_SYNC
  int pfd[2];
  if (pipe(pfd) < 0) {
    fprintf(stderr, "eyedbrc: cannot create unnamed pipes\n");
    return 1;
  }
#endif

  int pid;

  if ((pid = fork()) == 0) {
    ld_libpath_manage();
    std::string cmd = std::string(eyedbroot) + "/bin/eyedbd";
    std::string s = std::string("eyedbd");
    argv[0] = (char *)s.c_str();
#ifdef PIPE_SYNC
    close(pfd[0]);
    putenv(strdup((std::string("EYEDBPFD=") + str_convert((long)pfd[1])).c_str()));
#endif
    if (execve(cmd.c_str(), argv, environ) < 0) {
      kill(getppid(), SIGINT);
      fprintf(stderr, "eyedbrc: cannot execute '%s'\n", cmd.c_str());
    }

    exit(1);
  }

  if (pid < 0)
    return 1;

  close(pfd[1]);
  int status = 0;

#ifdef PIPE_SYNC
  if (read(pfd[0], &status, sizeof(status)) != sizeof(status)) {
    fprintf(stderr, "eyedbrc: eyedbd daemon start failed\n");
    return 1;
  }

  if (!status)
    fprintf(stderr, "Starting EyeDB Server\n"
	    " Version      V%s\n"
	    " Compiled     %s\n"
	    " Architecture %s\n"
	    " Program Pid  %d\n",
	    eyedb::getVersion(),
	    getCompilationTime(),
	    Architecture::getArchitecture()->getArch(),
	    pid);
#endif

  return status;
}

int
main(int argc, char *argv[])
{
  const char *idbport, *smdport, *logdir, *s, *idbserv;
  eyedbsm::Status status;
  Bool force = False;

  string sv_host, sv_port;
  eyedb::init(argc, argv, &sv_host, &sv_port, false);

  idbport = 0;
  logdir = 0;
  idbserv = 0;

  if (sv_port.length())
    idbport = sv_port.c_str();

  if (sv_host.length())
    idbserv = sv_port.c_str();

  if (argc < 2)
    return usage(argv[0]);

  Command cmd;

  if (!strcmp(argv[1], "start"))
    cmd = Start;
  else if (!strcmp(argv[1], "stop"))
    cmd = Stop;
  else if (!strcmp(argv[1], "status"))
    cmd = CStatus;
  else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
    help(argv[0]);
    return 0;
  }
  else
    return usage(argv[0]);

#if 1
  for (int i = 2; i < argc; i++) {
    char *s = argv[i];
    if (!strcmp(s, "-f"))
      force = True;
    else if (!strcmp(s, "-h") || !strcmp(s, "--help")) {
      help(argv[0]);
      return 0;
    }
  }
#else
  for (int i = 2; i < argc; i++) {
    char *s = argv[i];
    if (s[0] == '-') {
      if (!strcmp(s, "-f"))
	force = True;
      else if (!strcmp(s, "-sv_port")) {
	check("-sv_port", i, argc);
	idbport = argv[++i];
      }
      else if (!strcmp(s, "-sv_smdport")) {
	check("-sv_smdport", i, argc);
	smdport = argv[++i];
      }
      else if (!strcmp(s, "-sv_host")) {
	check("-sv_host", i, argc);
	idbserv = argv[++i];
      }
      else if (!strcmp(s, "-eyedbroot")) {
	check("-eyedbroot", i, argc);
	eyedbroot = argv[++i];
      }
      else if (!strcmp(s, "-eyedbconf")) {
	check("-eyedbconf", i, argc);
	Config::getDefaultConfig()->add(argv[++i]);
      }
    }
  }
#endif

  if (!idbport) {
    if (s = eyedb::getConfigValue("sv_port"))
      idbport = s;
    else
      idbport = Connection::DefaultIDBPortValue;
  }

  /*
  if (!smdport)
    smdport = eyedb::getConfigValue("sv_smdport");

  if (smdport)
    smd_set_port(smdport);
  */

  smdport = smd_get_port();

  // 14/02/05
  // @@@@@ should be bindir = eyedb::getConfigValue("bindir");
  if (!eyedbroot)
    eyedbroot = eyedb::getConfigValue("topdir");
  //eyedbroot = eyedb::getConfigValue("archdir");

  int ac;
  char **av;

  if (cmd == Start) {
    int st = force ? 1 : 0;

    //    string passwd_file = string(eyedbroot) + "/etc/eyedb/Password";
    string passwd_file = eyedb::getConfigValue("sv_passwd_file");
    if (access(passwd_file.c_str(), R_OK)) {
      fprintf(stderr, "\nPassword file '%s' is not accessible\n",
	      passwd_file.c_str());
      fprintf(stderr, "Did you run the post install script as follows:\n");
      fprintf(stderr, "sh %s/share/eyedb/tools/eyedb-postinstall.sh\n\n",
	      eyedbroot);
      return 1;
    }

    ac = argc - st;
    av = &argv[st+1];
  }

  SessionLog sesslog(idbport, eyedb::getConfigValue("sv_tmpdir"));

  if (sesslog.getStatus()) {
    if (cmd == Start)
      return startServer(ac, av, smdport);
    sesslog.getStatus()->print();
    return 1;
  }

  if (cmd == Start)
    return startServer(ac, av, smdport);

  if (cmd == Stop) {
    Status status = sesslog.stopServers(force);
    if (status) {
      status->print();
      return 1;
    }

    smdcli_conn_t *conn = smdcli_open(smd_get_port());
    if (!conn) {
      fprintf(stderr, "cannot connect to eyedbsmd daemon\n");
      return 1;
    }
    
    int r = smdcli_stop(conn);
    smdcli_close(conn);
    conn = 0;
    return r;
  }

  sesslog.display(stdout, force);

  return 0;
}
