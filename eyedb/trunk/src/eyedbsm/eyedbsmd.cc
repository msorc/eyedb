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


#define NO_IDB_LINKED_LIST

#include <config.h>

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include <eyedblib/rpc_lib.h>

#define USE_STL_LIST

#include <eyedblib/stdlist.h>

#include <eyedblib/semlib.h>
#include <eyedblib/filelib.h>
#include <eyedblib/filelib.h>

#include <kern_p.h>
#include <eyedbsm/smd.h>

#include <eyedblib/log.h>

using namespace eyedbsm;

static int smd_refcnt;

//#define TRACE
static void clean_exit();

class Reference {

protected:
  Reference() {
    refcnt = 1;
  }

  void incrRefCount() {
    refcnt++;
  }

  int decrRefCount() {
    return --refcnt;
  }

public:
  int getRefCount() const {
    return refcnt;
  }

private:
  int refcnt;
};

#ifdef UT_SEM
class Semaphore : public Reference {
  int key;
  int excl;
#ifdef USE_STL_LIST
  static std::list<Semaphore *> sem_list;
#else
  static LinkedList sem_list;
#endif

  Semaphore(int _key, int _excl) : Reference() {
    key = _key;
    excl = _excl;
#ifdef TRACE
    fprintf(stderr, "creating semaphore ");
    trace();
    fprintf(stderr, "\n");
#endif
#ifdef USE_STL_LIST
    sem_list.push_back(this);
#else
    sem_list.insertObject(this);
#endif
  }

public:
  int isExcl() const {
    return excl;
  }

  int getKey() const {
    return key;
  }

  void release() {
#ifdef TRACE
    fprintf(stderr, "releasing semaphore 0x%08x [refcnt:%d]\n", key, getRefCount());
#endif
    if (!decrRefCount()) {
#ifdef USE_STL_LIST
      bool r = std_list_erase(sem_list, this);
      if (!r)
	cerr << "Warning: semaphore::release " << key << "not found\n";
#else
      sem_list.deleteObject(this);
#endif
      delete this;
    }
  }

  ~Semaphore() {
#ifdef TRACE
    fprintf(stderr, "deleting key 0x%08x\n", key);
#endif
    ut_sem_rm(ut_sem_open(key));
  }

  void trace() {
    fprintf(stderr, "key 0x%08x [%s]", key, (excl ? "excl" : "shared"));
  }

  static Semaphore *find(int excl) {
    int key;
    Semaphore *sem;

    if (ut_sem_find(&key, excl) >= 0)
      return new Semaphore(key, excl);

#ifdef USE_STL_LIST
    std::list<Semaphore *>::const_iterator begin = sem_list.begin();
    std::list<Semaphore *>::const_iterator end = sem_list.end();

    while (begin != end ) {
      Semaphore *sem = *begin;
      if (!sem->isExcl()) {
	sem->incrRefCount();
	return sem;
      }
      ++begin;
    }
#else
    LinkedListCursor c(sem_list);

    while (c.getNext((void *&)sem))
      if (!sem->isExcl()) {
	sem->incrRefCount();
	return sem;
      }
#endif

#ifdef TRACE
    fprintf(stderr, "cannot find any semaphore\n");
#endif
    return 0;
  }

  static void traceList() {
#ifdef USE_STL_LIST
    std::list<Semaphore *>::const_iterator begin = sem_list.begin();
    std::list<Semaphore *>::const_iterator end = sem_list.end();

    while (begin != end ) {
      Semaphore *sem = *begin;
      sem->trace();
      ++begin;
    }
#else
    LinkedListCursor c(sem_list);
    Semaphore *sem;

    while (c.getNext((void *&)sem)) {
      sem->trace();
      fprintf(stderr, "\n");
    }
#endif
  }
};
#endif

class DbFile : public Reference {
  char *dbfile;
#ifdef UT_SEM
  Semaphore *sm[ESM_NSEMS];
#endif
#ifdef USE_STL_LIST
  static std::list<DbFile *> dbfile_list;
#else
  static LinkedList dbfile_list;
#endif

  DbFile(const char *_dbfile) : Reference() {
    dbfile = strdup(_dbfile);
#ifdef UT_SEM
    sm[0] = Semaphore::find(1);
    sm[1] = Semaphore::find(0);
#endif
#ifdef USE_STL_LIST
    dbfile_list.push_back(this);
#else
    dbfile_list.insertObject(this);
#endif
#ifdef TRACE
    fprintf(stderr, "creating dbfile ");
    trace();
    fprintf(stderr, "\n");
#endif
  }

  ~DbFile() {
#ifdef TRACE
    fprintf(stderr, "deleting dbfile %s\n", dbfile);
#endif
#ifdef UT_SEM
    for (int i = 0; i < ESM_NSEMS; i++)
      if (sm[i]) sm[i]->release();
#endif
  }

public:

#ifdef UT_SEM
  int getSemaphores(int sx[]) const {
    for (int i = 0; i < ESM_NSEMS; i++)
      sx[i] = (sm[i] ? sm[i]->getKey() : 0);

    return 0;
  }
#endif

  const char *getDbfile() const {
    return dbfile;
  }

  void release() {
#ifdef TRACE
    fprintf(stderr, "releasing dbfile %s [refcnt:%d]\n", dbfile, getRefCount());
#endif
    if (!decrRefCount()) {
#ifdef USE_STL_LIST
      bool r = std_list_erase(dbfile_list, this);
#ifdef TRACE
      if (!r)
	cerr << "Warning: DbFile::release " << dbfile << "not found\n";
#endif
#else
      dbfile_list.deleteObject(this);
#endif
      delete this;
    }
  }

  static DbFile *find(const char *dbfile, int get_sems, int &found) {
#ifdef USE_STL_LIST
    std::list<DbFile *>::const_iterator begin = dbfile_list.begin();
    std::list<DbFile *>::const_iterator end = dbfile_list.end();
    DbFile *dbf;

    while (begin != end) {
      dbf = *begin;
      if (!strcmp(dbf->dbfile, dbfile)) {
	if (get_sems)
	  dbf->incrRefCount();
	found = 1;
	return dbf;
      }
      ++begin;
    }
#else
    LinkedListCursor c(dbfile_list);
    DbFile *dbf;

    while (c.getNext((void *&)dbf))
      if (!strcmp(dbf->dbfile, dbfile)) {
	if (get_sems)
	  dbf->incrRefCount();
	found = 1;
	return dbf;
      }
#endif

    found = 0;
    if (get_sems) {
      dbf = new DbFile(dbfile);
      /*
      time_t t0 = time(0);
      fprintf(stderr, "Cleaning database %s at %s", dbfile, ctime(&t0));
      fflush(stderr);
      */
      
      Status s = dbCleanup(dbfile);
      if (s && s->err != CANNOT_LOCK_SHMFILE)
	statusPrint(s, "eyedbsmd");
#ifdef TRACE
      if (s)
	statusPrint(s, "eyedbsmd");
#endif
      return dbf;
    }
    return 0;
  }

  void trace() {
    fprintf(stderr, "%s, ", dbfile);
#ifdef UT_SEM
    for (int i = 0; i < ESM_NSEMS; i++) {
      if (i) fprintf(stderr, ", ");
      if (!sm[i]) fprintf(stderr, "*");
      else sm[i]->trace();
    }
#endif
  }

  static void traceList() {
#ifdef USE_STL_LIST
    std::list<DbFile *>::const_iterator begin = dbfile_list.begin();
    std::list<DbFile *>::const_iterator end = dbfile_list.end();
    DbFile *dbf;

    while (begin != end) {
      dbf = *begin;
      dbf->trace();
      fprintf(stderr, "\n");
      ++begin;
    }
#else
    LinkedListCursor c(dbfile_list);
    DbFile *dbf;

    while (c.getNext((void *&)dbf)) {
      dbf->trace();
      fprintf(stderr, "\n");
    }
#endif
  }
};

class Client {
  int fd;
#ifdef USE_STL_LIST
  static std::list<Client *> client_list;
  std::list<DbFile *> dbfile_list;
#else
  static LinkedList client_list;
  LinkedList dbfile_list;
#endif

public:
  Client(int _fd) {
    fd = _fd;
#ifdef TRACE
    fprintf(stderr, "creating client %d\n", fd);
#endif
#ifdef USE_STL_LIST
    client_list.push_back(this);
#else
    client_list.insertObject(this);
#endif
  }

  void addDbFile(DbFile *dbf) {
#ifdef USE_STL_LIST
    dbfile_list.push_back(dbf);
#else
    dbfile_list.insertObject(dbf);
#endif

#ifdef TRACE
    fprintf(stderr, "adding dbfile %s to client %d [refcnt:%d]\n", dbf->getDbfile(), fd,
	   dbf->getRefCount());
#endif
  }

  void rmDbFile(DbFile *dbf) {
#ifdef TRACE
    fprintf(stderr, "removing dbfile %s from client %d\n", dbf->getDbfile(), fd);
#endif
#ifdef USE_STL_LIST
    std::list<DbFile *>::iterator db_begin = dbfile_list.begin();
    std::list<DbFile *>::iterator db_end = dbfile_list.end();
    while (db_begin != db_end) {
      DbFile *xdbf = *db_begin;
      if (xdbf == dbf) {
	dbf->release();
	bool r = std_list_erase(dbfile_list, dbf);
#ifdef TRACE
	if (!r)
	  cerr << "Warning: rmDbFile not found\n";
#endif
	return;
      }
      ++db_begin;
    }
#else
    LinkedListCursor c(dbfile_list);
    DbFile *xdbf;
    while (c.getNext((void *&)xdbf))
      if (xdbf == dbf) {
	dbf->release();
	dbfile_list.deleteObject(dbf);
	return;
      }
#endif

#ifdef TRACE
    fprintf(stderr, "dbfile %s not found in client %d\n", dbf->getDbfile(), fd);
#endif
  }

  ~Client() {
#ifdef TRACE
    fprintf(stderr, "releasing client %d\n", fd);
#endif
#ifdef USE_STL_LIST
    std::list<DbFile *>::iterator db_begin = dbfile_list.begin();
    std::list<DbFile *>::iterator db_end = dbfile_list.end();
    while (db_begin != db_end) {
      DbFile *dbf = *db_begin;
      dbf->release();
      ++db_begin;
    }
    bool r = std_list_erase(client_list, this);
#ifdef TRACE
    if (!r)
      cerr << "Warning: Client not found\n";
#endif
#else
    LinkedListCursor c(dbfile_list);
    DbFile *dbf;
    while (c.getNext((void *&)dbf))
      dbf->release();

    client_list.deleteObject(this);
#endif
  }

  static void clean_all() {
#ifdef USE_STL_LIST
    std::list<Client *>::iterator cl_begin = client_list.begin();
    std::list<Client *>::iterator cl_end = client_list.end();

    while (cl_begin != cl_end) {
      Client *client = *cl_begin;
      std::list<DbFile *>::iterator db_begin = client->dbfile_list.begin();
      std::list<DbFile *>::iterator db_end = client->dbfile_list.end();
      while (db_begin != db_end) {
	DbFile *dbf = *db_begin;
	dbf->release();
	++db_begin;
      }
      ++cl_begin;
    }
#else
    LinkedListCursor c(client_list);
    Client *client;

    while (c.getNext((void *&)client)) {
      LinkedListCursor c(client->dbfile_list);
      DbFile *dbf;
      while (c.getNext((void *&)dbf))
	dbf->release();
    }
#endif
  }

  static Client *find(int fd) {
#ifdef USE_STL_LIST
    std::list<Client *>::iterator begin = client_list.begin();
    std::list<Client *>::iterator end = client_list.end();
    while (begin != end) {
      Client *client = *begin;
      if (client->fd == fd)
	return client;
      ++begin;
    }
#else
    LinkedListCursor c(client_list);
    Client *client;

    while (c.getNext((void *&)client))
      if (client->fd == fd)
	return client;

#endif
    return 0;
  }
};

#ifdef USE_STL_LIST
std::list<DbFile *> DbFile::dbfile_list;
#else
LinkedList DbFile::dbfile_list;
#endif

#ifdef UT_SEM

#ifdef USE_STL_LIST
std::list<Semaphore *> Semaphore::sem_list;
#else
LinkedList Semaphore::sem_list;
#endif

#endif
#ifdef USE_STL_LIST
std::list<Client *> Client::client_list;
#else
LinkedList Client::client_list;
#endif

static void
unit_test()
{
  DbFile *dbf1, *dbf2, *dbf3;
  int found;
  dbf1 = DbFile::find("foo.dbs", 1, found);
  dbf2 = DbFile::find("goo.dbs", 1, found);
  dbf3 = DbFile::find("foo.dbs", 1, found);

  DbFile::traceList();

  fprintf(stderr, "continue> ");
  getchar();

  dbf1->release();
  dbf2->release();
  dbf3->release();
}

static struct sockaddr_un sock_un_name;

static int
port_open(const char *port)
{
  int sockun_fd;
  int v = 1;

  if ((sockun_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "unable to create unix socket port `%d'\n", port);
    return -1;
  }
  
  if (setsockopt(sockun_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&v, sizeof(v)) <
      0) {
    fprintf(stderr, "setsockopt reuseaddr\n");
    return -1;
  }

  sock_un_name.sun_family = AF_UNIX;
  strcpy(sock_un_name.sun_path, port);

  if (bind(sockun_fd, (struct sockaddr *)&sock_un_name,
	   sizeof(sock_un_name)) < 0 ) {
    fprintf(stderr, "bind: failing on port %s (%s)\n", port,
	    strerror(errno));
    fprintf(stderr, "\nPerharps another eyedbsmd is running on port:\n%s\n",
	    port);
    fprintf(stderr, "\nIf no, unlink this port as follows:\n");
    fprintf(stderr, "rm -f %s\n", port);
    fprintf(stderr, "and relaunch the server.\n");
    return -1;
  }

  chmod(port, 0777);
  if (sockun_fd >= 0 && listen(sockun_fd, 2) < 0 ) {
    fprintf(stderr, "listen: failing on port %s (%s)\n", port,
	    strerror(errno));
    return -1;
  }

  return sockun_fd;
}

static void
clear_client(fd_set &fds, int fd, int &max_fd)
{
  Client *client = Client::find(fd);
  delete client;
  FD_CLR(fd, &fds);
  close(fd);
  if (max_fd == fd)
    --max_fd;
}

static const char *
string_message(int msg)
{
  if (msg == SMD_INIT)
    return "SMD_INIT";

  if (msg == SMD_INIT_GETSEMS)
    return "SMD_INIT_GETSEMS";

  if (msg == SMD_RELEASE)
    return "SMD_RELEASE";

  if (msg == SMD_STATUS)
    return "SMD_STATUS";

  if (msg == SMD_DECL)
    return "SMD_DECL";

  if (msg == SMD_UNDECL)
    return "SMD_UNDECL";

  if (msg == SMD_STOP)
    return "SMD_STOP";

  return "UNKNOWN";
}

static int
manage_message(int fd)
{
  int msg;
  Client *client = Client::find(fd);
  if (!client)
    return 0;

  if (rpc_socketRead(fd, &msg, sizeof(msg)) != sizeof(msg))
    return 0;

  /*
  time_t t0 = time(0);
  fprintf(stderr, "Getting Message '%s' at %s", string_message(msg),
	  ctime(&t0));
  fflush(stderr);
  */

  if (msg == SMD_INIT || msg == SMD_INIT_GETSEMS || msg == SMD_RELEASE) {
    int len;
    if (rpc_socketRead(fd, &len, sizeof(len)) != sizeof(len))
      return 0;

    char *dbfile = new char[len];
    if (rpc_socketRead(fd, dbfile, len) != len)
      return 0;

    int found;
    DbFile *dbf = DbFile::find(dbfile, (msg == SMD_INIT_GETSEMS ||
					msg == SMD_INIT), found);
    delete [] dbfile;
    
    if (msg == SMD_INIT_GETSEMS) {
#ifdef UT_SEM
      client->addDbFile(dbf);

      int sm[ESM_NSEMS];
      dbf->getSemaphores(sm);
      
      if (rpc_socketWrite(fd, sm, sizeof(int)*ESM_NSEMS) !=
	  sizeof(int)*ESM_NSEMS)
	return 0;
#else
      assert(0);
#endif
    }
    else if (msg == SMD_INIT) {
      client->addDbFile(dbf);
      if (rpc_socketWrite(fd, &msg, sizeof(msg)) != sizeof(msg))
	return 0;
    }
    else
      client->rmDbFile(dbf);
  }
  else if (msg == SMD_STATUS) {
    fprintf(stderr, "Reference Count: %d\n", smd_refcnt);
    DbFile::traceList();
#ifdef UT_SEM
    Semaphore::traceList();
#endif
  }
  else if (msg == SMD_DECL)
    smd_refcnt++;
  else if (msg == SMD_UNDECL) {
    if (smd_refcnt > 0)
      --smd_refcnt;
  }
  else if (msg == SMD_STOP) {
#ifdef TRACE
    fprintf(stderr, "smd_refcnt %d\n", smd_refcnt);
#endif
    Client::clean_all();
    clean_exit();
    exit(0);
  }
  else {
    fprintf(stderr, "unknown message %x\n", msg);
    return 0;
  }

  return 1;
}

static void
net_main_loop(int sock_fd)
{
  fd_set fds;
  int max_fd = sock_fd, fd;

  FD_ZERO(&fds);
  FD_SET(sock_fd, &fds);
  
  for (;;) {
#ifdef TRACE
    fprintf(stderr, "\nwaiting on socket %d, maxfd %d\n", sock_fd, max_fd);
#endif
    fd_set fdt = fds;
    if (select (max_fd+1, &fdt, 0, 0, 0) < 0) {
      perror("select");
      continue;
    }

    for (fd = 0; fd <= max_fd; fd++)
      if (FD_ISSET(fd, &fdt))  {
	if (fd == sock_fd) {
	  struct sockaddr *sock_addr;
#if defined(ORIGIN) || defined(ALPHA)
	  int length;
#else
	  socklen_t length;
#endif
	  int input_fd;

	  sock_addr = (struct sockaddr *)&sock_un_name;
	  length = sizeof(sock_un_name);

	  if ((input_fd = accept(fd, sock_addr, &length)) < 0) {
	    perror("accept");
	    continue;
	  }

	  new Client(input_fd);
	  FD_SET(input_fd, &fds);
	  if (input_fd > max_fd)
	    max_fd = input_fd;
	}
	else if (!manage_message(fd)) {
	  clear_client(fds, fd, max_fd);
	  continue;
	}
      }
  }
}

static const char *port;

static void
clean_exit()
{
  unlink(port);
}

static void
signal_handler(int sig) 
{
  clean_exit();
  exit(sig);
}

static int
notice(int status)
{
  const char *s;
  if (s = getenv("EYEDBPFD")) {
    int pfd = atoi(s);
    if (pfd > 0) {
      write(pfd, &status, sizeof(status));
      close(pfd);
    }
  }

  return status;
}

#include <limits.h>

static void
init()
{
  /*@@@@ #if !defined(LINUX) && !defined(CYGWIN) */
#if defined(SOLARIS) || defined(ULTRASOL7)  
  struct rlimit rlp;

  if (!getrlimit(RLIMIT_NOFILE, &rlp)) {
    rlp.rlim_cur = rlp.rlim_max;
    setrlimit(RLIMIT_NOFILE, &rlp);
  }
#endif

  signal(SIGHUP,  SIG_IGN);
  signal(SIGINT,  SIG_IGN);
  signal(SIGTERM, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGSEGV, signal_handler);
  signal(SIGBUS,  signal_handler);
  signal(SIGABRT, signal_handler);
}

static int
usage(const char *prog) {
  fprintf(stderr, "usage: %s [-p <port>|--port=<port>] [--status] [--stop]\n", prog);
  return notice(1);
}

int
main(int argc, char *argv[])
{
  enum Action {
    Daemon = 1,
    Status,
    Stop
  } action;

  action = Daemon;

  static const char port_opt[] = "--port=";
  unsigned int port_len = strlen(port_opt);

  for (int n = 1; n < argc; n++) {
    const char *s = argv[n];
    if (*s == '-') {
      if (!strcmp(s, "-p")) {
	if (n == argc-1)
	  return usage(argv[0]);
	smd_set_port(argv[++n]);
      }	
      else if (!strncmp(s, port_opt, port_len)) {
	smd_set_port(&argv[n][port_len]);
      }	
      else if (!strcmp(s, "--status")) {
	action = Status;
      }
      else if (!strcmp(s, "--stop")) {
	action = Stop;
      }
      else
	return usage(argv[0]);
    }
    else
      return usage(argv[0]);
  }

  if (action != Daemon) {
    smdcli_conn_t *conn = smdcli_open(smd_get_port());

    if (!conn)
      return 1;

    if (action == Stop)
      return smdcli_stop(conn);

    return smdcli_status(conn);
  }

  const char *logmask = getenv("IDB_LOG_MASK");
  if (logmask) {
    utlogInit(argv[0], "stderr");
    sscanf(logmask, "%llx", &eyedblib::log_mask);
  }

  port = smd_get_port();

  eyedbsm::init();
  ::init();

  int sock_fd = port_open(port);
  if (sock_fd < 0)
    return notice(1);

  smd_refcnt = 1;
  (void)notice(0);

  net_main_loop(sock_fd);

  clean_exit();
  return notice(0);
}
