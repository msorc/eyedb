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


// 3/6/05: should be splitted into opts.cc, init.cc, version.cc

#include "eyedb_p.h"
#include "eyedb/DBM_Database.h"
#include "eyedb/GenHashTable.h"
#include <eyedblib/m_malloc.h>
#include <eyedbsm/smd.h>
#include "eyedbversion.h"
#include "oqlctb.h"
#include <eyedb/ThreadPoolManager.h>
#include "comp_time.h"
#include "GetOpt.h"
#include <sstream>

#define USE_GETOPT

using namespace std;

extern int RPC_MIN_SIZE;

/*
p_ProbeHandle *eyedb_probe_h;
static const char *eyedb_probe_name = getenv("EYEDBPROBE");
*/

extern void (*rpc_release_all)(void);

// EYEDB_VERSION defined in eyedbversion.h
namespace eyedb {

  static const char eyedb_version[] = EYEDB_VERSION;

  static void
  stop_on_error(Status status, void *)
  {
    fprintf(stderr, "<<< catching eyedb error >>>\n");
    status->print(stderr);

    fprintf(stderr, "continue> ");

    while (getchar() != '\n')
      ;
  }

  static void
  echo_on_error(Status status, void *)
  {
    fprintf(stderr, "<<< catching eyedb error >>>\n");
    status->print(stderr);
  }

  static void
  abort_on_error(Status status, void *)
  {
    fprintf(stderr, "<<< catching eyedb error >>>\n");
    status->print(stderr);
    abort();
  }

  static char *LogName;
  static int LogLevel;

  inline static void check(const char *prog, const char *opt, int i, int argc,
			   char *argv[])
  {
    if (i >= argc - 1 || *argv[i+1] == '-')
      {
	fprintf(stderr, "%s: missing argument after option '%s'\n", prog, opt);
	exit(1);
      }
  }

  void
  printVersion()
  {
    printf("EyeDB Copyright (C) 1994-1999,2004-2006 SYSRA\n");
    printf(" Version      V%s\n", eyedb_version);
    printf(" Compiled     %s\n", getCompilationTime());
    printf(" Architecture %s\n", Architecture::getArchitecture()->getArch());
    exit(0);
  }

  char *prog_name = "<unknown>";

  static void
  ask_for_user()
  {
    char userauth[32];
    fprintf(stderr, "user authentication: ");
    fflush(stderr);
    if (!fgets(userauth, sizeof(userauth)-1, stdin))
      exit(1);
    userauth[strlen(userauth)-1] = 0;
    Connection::setDefaultUser(userauth);
  }

  static void
  ask_for_passwd()
  {
    Connection::setDefaultPasswd(getpass("password authentication: "));
  }

#define ON_OFF(OPT, MTH) \
do { \
  check(argv[0], "-" OPT, i, argc, argv); \
  const char *sx = argv[++i]; \
  if (!strcmp(sx, "on")) \
    Log::MTH(True); \
  else if (!strcmp(sx, "off")) \
    Log::MTH(False); \
  else \
    { \
      fprintf(stderr, "-%s option: 'on' or 'off' expected\n", OPT); \
      exit(1); \
    } \
  i++; \
} while(0)

  static void
  make_options(int &argc, char *argv[], ostream *usage_ostr,
	       std::ostream *help_ostr, string *listen = 0,
	       bool purgeargv = true)
  {
    int n = 1;

    static const char *opt_prefix = "-eyedb";
    static int len_opt_prefix = strlen(opt_prefix);
    static const char *opt_sv_prefix = "-eyedbsv";
    static int len_opt_sv_prefix = strlen(opt_sv_prefix);

    // disconnect this condition because it should work also for
    // local opening mode
    /*if (listen)*/ { // means : server mode
#if 0
      const char *smdport = ServerConfig::getSValue("smdport");
      if (smdport)
	smd_set_port(smdport);
#endif
    }

    if (getenv("RPC_MIN_SIZE")) {
      RPC_MIN_SIZE = atoi(getenv("RPC_MIN_SIZE"));
      printf("RPC_MIN_SIZE %u\n", RPC_MIN_SIZE);
    }

    const std::string prefix = "";
    std::vector<string> error_v;
    error_v.push_back("status");
    error_v.push_back("exception");
    error_v.push_back("abort");
    error_v.push_back("stop");
    error_v.push_back("echo");
    OptionChoiceType errorChoice("error", error_v, "status");

    static const std::string user_opt = "user";
    static const std::string passwd_opt = "passwd";
    static const std::string auth_opt = "auth";
    static const std::string listen_opt = "listen";
    static const std::string host_opt = "host";
    static const std::string port_opt = "port";
    static const std::string inet_opt = "inet";
    static const std::string smd_port_opt = "smd-port";
    static const std::string dbm_opt = "dbm";
    static const std::string granted_dbm_opt = "granted-dbm";
    static const std::string default_dbm_opt = "default-dbm";
    static const std::string conf_opt = "conf";
    static const std::string server_conf_opt = "server-conf";
    static const std::string logdev_opt = "logdev";
    static const std::string logmask_opt = "logmask";
    static const std::string logdate_opt = "logdate";
    static const std::string logtimer_opt = "logtimer";
    static const std::string logpid_opt = "logpid";
    static const std::string logprog_opt = "logprog";
    static const std::string error_policy_opt = "error-policy";
    static const std::string trans_def_mag_opt = "trans-def-mag";
    static const std::string arch_opt = "arch";
    static const std::string version_opt = "version";
    static const std::string help_eyedb_options_opt = "help-eyedb-options";

    Option opts[32];

    unsigned int opt_cnt = 0;

    opts[opt_cnt++] = 
      Option('U', prefix + user_opt, OptionStringType(),
	     Option::MandatoryValue, OptionDesc("User name", "<user>|@"));

    opts[opt_cnt++] = 
      Option('P', prefix + passwd_opt, OptionStringType(),
	     Option::OptionalValue,  OptionDesc("Password", "<passwd>"));

    if (listen) {
      opts[opt_cnt++] = 
	Option(listen_opt, OptionStringType(),
	       Option::MandatoryValue,
	       OptionDesc("listen host and ports", "[<host>:]<port>"));

      opts[opt_cnt++] = 
	Option(prefix + granted_dbm_opt, OptionStringType(),
	       Option::MandatoryValue,
	       OptionDesc("Granted EYEDBDBM database files", "<dbmfiles>"));

      opts[opt_cnt++] = 
	Option(prefix + default_dbm_opt, OptionStringType(),
	       Option::MandatoryValue,
	       OptionDesc("Default EYEDBDBM database file", "<dbmfile>"));
    }
    else {
      opts[opt_cnt++] = 
	Option(prefix + host_opt, OptionStringType(), Option::MandatoryValue,
	       OptionDesc("eyedbd host", "<host>"));

      opts[opt_cnt++] = 
	Option(prefix + port_opt, OptionStringType(), Option::MandatoryValue,
	       OptionDesc("eyedbd port", "<port>"));

      opts[opt_cnt++] = 
	Option(prefix + inet_opt, OptionStringType(), 0,
	       OptionDesc("Use the tcp_port variable if port is not set"));

      opts[opt_cnt++] = 
	Option(prefix + dbm_opt, OptionStringType(), Option::MandatoryValue,
	       OptionDesc("EYEDBDBM database file", "<dbmfile>"));
    }


    opts[opt_cnt++] = 
      Option(prefix + conf_opt, OptionStringType(), Option::MandatoryValue,
	     OptionDesc("Client Configuration file", "<conffile>"));

    opts[opt_cnt++] = 
      Option(prefix + server_conf_opt, OptionStringType(),
	     Option::MandatoryValue,
	     OptionDesc(std::string("Server Configuration file") +
			(listen ? "" : " (used for local opening)"),
			"<conffile>"));

    opts[opt_cnt++] = 
      Option(prefix + smd_port_opt, OptionStringType(), Option::MandatoryValue,
	     OptionDesc(std::string("eyedbsmd port") +
			(listen ? "" : " (used for local opening)"),
			"<port>"));
    
    opts[opt_cnt++] = 
      Option(prefix + logdev_opt, OptionStringType(), Option::MandatoryValue,
	     OptionDesc("Output log file", "<logfile>"));

    opts[opt_cnt++] = 
      Option(prefix + logmask_opt, OptionStringType(), Option::MandatoryValue,
	     OptionDesc("Output log mask", "<mask>"));

    opts[opt_cnt++] = 
      Option(prefix + logdate_opt, OptionBoolType(), Option::MandatoryValue,
	     OptionDesc("Control date display in output log", "on|off"));

    opts[opt_cnt++] = 
      Option(prefix + logtimer_opt, OptionBoolType(), Option::MandatoryValue,
	     OptionDesc("Control timer display in output log", "on|off"));

    opts[opt_cnt++] = 
      Option(prefix + logpid_opt, OptionBoolType(), Option::MandatoryValue,
	     OptionDesc("Control pid display in output log", "on|off"));

    opts[opt_cnt++] = 
      Option(prefix + logprog_opt, OptionBoolType(), Option::MandatoryValue,
	     OptionDesc("Control progname display in output log", "on|off"));

    opts[opt_cnt++] = 
      Option(prefix + error_policy_opt, errorChoice, Option::MandatoryValue,
	     OptionDesc("Control error policy: status|exception|abort|stop|echo",
			"<value>"));
    opts[opt_cnt++] = 
      Option(prefix + trans_def_mag_opt, OptionIntType(), Option::MandatoryValue,
	     OptionDesc("Default transaction magnitude order",
			"<magorder>"));

    opts[opt_cnt++] = 
      Option(prefix + arch_opt, OptionBoolType(), 0,
	     OptionDesc("Display the client architecture"));

    opts[opt_cnt++] = 
      Option('v', prefix + version_opt, OptionBoolType(), 0,
	     OptionDesc("Display the version"));

    opts[opt_cnt++] = 
      Option(prefix + help_eyedb_options_opt, OptionBoolType(), 0,
	     OptionDesc("Display this message"));

    GetOpt getopt(argv[0], opts, opt_cnt,
		  GetOpt::SkipUnknownOption|(purgeargv ? GetOpt::PurgeArgv : 0));

    if (usage_ostr) {
      getopt.usage("", "");
      return;
    }

    if (help_ostr) {
      getopt.help(*help_ostr, "  ");
      return;
    }

    if (!getopt.parse(argc, argv)) {
      getopt.usage("\n");
      exit(0);
    }

    prog_name = strdup(argv[0]);

    GetOpt::Map &map = getopt.getMap();

    if (map.find(help_eyedb_options_opt) != map.end()) {
      getopt.help(cerr, "  ");
      exit(0);
    }

    if (map.find(conf_opt) != map.end()) {
      Status s = ClientConfig::setConfigFile(map[conf_opt].value.c_str());
      if (s) {
	s->print(stderr);
	exit(1);
      }
    }

    if (map.find(server_conf_opt) != map.end()) {
      Status s = ServerConfig::setConfigFile(map[server_conf_opt].value.c_str());
      if (s) {
	s->print(stderr);
	exit(1);
      }
    }

    if (map.find(port_opt) != map.end())
      Connection::setDefaultIDBPort(map[port_opt].value.c_str());
    else if (map.find(inet_opt) != map.end())
      Connection::setDefaultIDBPort(ClientConfig::getCValue("tcp_port"));

    if (map.find(host_opt) != map.end())
      Connection::setDefaultHost(map[host_opt].value.c_str());

    if (map.find(listen_opt) != map.end()) {
      assert(listen);
      if (listen)
	*listen = map[listen_opt].value;
      else
	getopt.usage("\n");

      ServerConfig::getInstance()->setValue("listen",
					    map[listen_opt].value.c_str());
    }

    if (map.find(granted_dbm_opt) != map.end())
      ServerConfig::getInstance()->setValue("granted_dbm",
					    map[granted_dbm_opt].value.c_str());

    if (map.find(default_dbm_opt) != map.end())
      ServerConfig::getInstance()->setValue("default_dbm",
					    map[default_dbm_opt].value.c_str());

    if (map.find(dbm_opt) != map.end())
      Database::setDefaultDBMDB(map[dbm_opt].value.c_str());

    if (map.find(user_opt) != map.end()) {
      if (map[user_opt].value.length() == 0)
	ask_for_user();
      else
	  Connection::setDefaultUser(map[user_opt].value.c_str());
    }

    if (map.find(passwd_opt) != map.end()) {
      if (map[passwd_opt].value.length() == 0)
	ask_for_passwd();
      else
	Connection::setDefaultPasswd(map[passwd_opt].value.c_str());
    }

    if (map.find(auth_opt) != map.end()) {
      if (map[auth_opt].value.length() == 0) {
	ask_for_user();
	ask_for_passwd();
      }
      else
	Connection::setDefaultPasswd(map[user_opt].value.c_str());
    }

    if (map.find(version_opt) != map.end()) {
      printVersion();
      exit(0);
    }

    if (map.find(arch_opt) != map.end()) {
      printf("%s\n", Architecture::getArchitecture()->getArch());
      exit(0);
    }

    if (map.find(logdev_opt) != map.end())
      LogName = strdup(map[logdev_opt].value.c_str());

    if (map.find(logpid_opt) != map.end()) {
      bool b = ((OptionBoolType *)map[logpid_opt].type)->getBoolValue
	(map[logpid_opt].value);
      Log::setLogPid(b ? True : False);
    }

    if (map.find(logdate_opt) != map.end()) {
      bool b = ((OptionBoolType *)map[logdate_opt].type)->getBoolValue
	(map[logdate_opt].value);
      Log::setLogDate(b ? True : False);
    }

    if (map.find(logtimer_opt) != map.end()) {
      bool b = ((OptionBoolType *)map[logtimer_opt].type)->getBoolValue
	(map[logtimer_opt].value);
      Log::setLogTimer(b ? True : False);
    }

    if (map.find(logprog_opt) != map.end()) {
      bool b = ((OptionBoolType *)map[logprog_opt].type)->getBoolValue
	(map[logprog_opt].value);
      Log::setLogProgName(b ? True : False);
    }

    if (map.find(logmask_opt) != map.end()) {
      Status s = Log::setLogMask(map[logmask_opt].value.c_str());
      if (s) {
	s->print(stderr);
	exit(1);
      }
    }

    const char *smdport = ServerConfig::getSValue("smdport");
    if (smdport)
      smd_set_port(smdport);

    if (map.find(smd_port_opt) != map.end()) {
      smd_set_port(map[smd_port_opt].value.c_str());
      ServerConfig::getInstance()->setValue("smdport",
					    map[smd_port_opt].value.c_str());
    }

    if (map.find(trans_def_mag_opt) != map.end())
      TransactionParams::setGlobalDefaultMagOrder(atoi(map[trans_def_mag_opt].value.c_str()));
 
    if (map.find(error_policy_opt) != map.end()) {
      const char *policy = map[error_policy_opt].value.c_str();

      if (!strcmp(policy, "status"))
	Exception::setMode(Exception::StatusMode);
      else if (!strcmp(policy, "exception"))
	Exception::setMode(Exception::ExceptionMode);
      else if (!strcmp(policy, "abort"))
	Exception::setHandler(abort_on_error);
      else if (!strcmp(policy, "stop"))
	Exception::setHandler(stop_on_error);
      else if (!strcmp(policy, "echo"))
	Exception::setHandler(echo_on_error);
      else
	exit(1);
    }

    Connection::init();
  }

   void print_standard_usage(GetOpt &getopt, const std::string &append,
			     ostream &os, bool server)
  {
    getopt.usage(" " + append, "usage: ", os);
    os << "\n\nCommon Options:\n";
    print_common_usage(os, server);
    os << '\n';
  }


  void print_standard_help(GetOpt &getopt,
			   const std::vector<std::string> &options,
			   ostream &os, bool server)
  {
    os << "Program Options:\n";
    getopt.help(os, "  ");
    unsigned int size = options.size();
    for (unsigned int n = 0; n < size; n += 2)
      getopt.helpLine(options[n], options[n+1], os);
    os << "\nCommon Options:\n";
    print_common_help(os, server);
  }


  void print_common_usage(ostream &os, bool server)
  {
    int argc = 1;
    char *argv[] = {""};
    string listen;
    make_options(argc, argv, &os, 0, server ? &listen : 0);
  }

  void print_common_help(ostream &os, bool server)
  {
    int argc = 1;
    char *argv[] = {""};
    string listen;
    make_options(argc, argv, 0, &os, server ? &listen : 0);
  }

#if 0
  const char *
  getStdOptionsUsage()
  {
    return "[-eyedbhost <host>] [-eyedbport <portname>] [-eyedbsmdport <smdport>] [-eyedbdbm <dbmfile>] [-eyedbuser <user>] [-eyedbuser?] [-eyedbpasswd <passwd>] [-eyedbpasswd?] [-eyedbauth?] [-eyedbconf <conffile>] [-eyedblog <logfile>] [-eyedblogmask <mask>] [-eyedblogpid on|off] [-eyedblogdate on|off] [-eyedblogtimer on|off] [-eyedblogprog on|off ] [-eyedb-error-policy status|exception|abort|stop|echo] [-eyedbarch] [-eyedbtransless] [-eyedbtransdefmgo <magorder>] [-eyedbversion]";
  }

  const char *
  getSrvOptionsUsage()
  {
    return "[-sv_port <ports>] [-sv_host <hostname>] [-sv_access_file <accessfile>] [-sv_timeout <timeout>] [-sv_datdir <datdir>] [-sv_passwd_file <passwdfile>] [-sv_nod] [-eyedbdbm <dbmfile>] [-eyedbconf <conffile>] [-eyedblog <logfile>] [-eyedblogmask <mask>] [-eyedblogpid on|off] [-eyedblogdate on|off] [-eyedblogtimer on|off] [-eyedblogprog on|off ] [-eyedbarch] [-eyedbtransless] [-eyedbtransdefmgo <magorder>] [-eyedbversion] [-help]";
  }


  const char *
  getStdOptionsHelp(const char *indent)
  {
    static char buf[2048];
    sprintf(buf,
	    "\neyedb standard options:\n"
	    "%s-eyedbhost <host>             eyedb host server\n"
	    "%s-eyedbport <port>             eyedb port name or number\n"
	    "%s-eyedbsmdport <port>          smd port name [default is %s]\n"
	    "%s-eyedbdbm <dbmfile>           EYEDBDBM database file\n"
	    "%s-eyedbuser <user>             eyedb user name\n"
	    "%s-eyedbpasswd <passwd>         eyedb password\n"
	    "%s-eyedbuser?                   asks for eyedb user name on stdin\n"
	    "%s-eyedbpasswd?                 asks for eyedb password on stdin\n"
	    "%s-eyedbauth?                   asks for eyedb user/password on stdin\n"
	    "%s-eyedbconf <conffile>         configuration file\n"
	    "%s-eyedblog <logfile>           output log file\n"
	    "%s-eyedblogmask <mask>          output log mask\n"
	    "%s-eyedblogdate on|off          controls date display in output log\n"
	    "%s-eyedblogtimer on|off         controls timer display in output log\n"
	    "%s-eyedblogpid on|off           controls pid display in output log\n"
	    "%s-eyedblogprog on|off          controls progname display in output log\n"
	    "%s-eyedb-error-policy status    returns a status on any eyedb error\n"
	    "%s-eyedb-error-policy exception throws an exception on any eyedb error\n"
	    "%s-eyedb-error-policy abort     aborts the program on any eyedb error\n"
	    "%s-eyedb-error-policy stop      stops the program on any eyedb error\n"
	    "%s-eyedb-error-policy echo      displays each error on the standard error\n"
	    "%s-eyedbtransless               default transaction mode is less\n"
	    "%s-eyedbtransdefmgo <magorder>  sets the default transaction magnitude order\n"
	    "%s-eyedbarch                    displays the client architecture\n"
	    "%s-eyedbversion                 displays the eyedb version\n"
	    "%s-eyedbhelp                    displays this message\n",
	    indent, indent, indent, indent, smd_get_port(),
	    indent, indent, indent, indent,
	    indent, indent, indent, indent, indent, indent, indent, indent,
	    indent, indent, indent, indent, indent, indent, indent, indent,
	    indent, indent, indent, indent, indent, indent, indent, indent);

    return buf;
  }
#endif

#include <pthread.h>

  static void
  checkLinkedWithMt()
  {
    if (getenv("EYEDBNOMT"))
      return;

    pthread_mutex_t mp;
    pthread_mutex_init(&mp, NULL);
    pthread_mutex_lock(&mp);

    if (!pthread_mutex_trylock(&mp))
      {
	fprintf(stderr, "eyedb fatal error: this program has not been linked "
		"with the thread library: flag -mt or -lpthread\n");
	exit(1);
      }
  }

  static FILE *fd_stream;
  static char file_stream[128];

  static void
  stream_init()
  {
    if (fd_stream)
      return;

    const char *tmpdir = ServerConfig::getSValue("tmpdir");
    if (!tmpdir)
      tmpdir = "/tmp";
    char *s = tempnam(tmpdir, "eyedb_");
    strcpy(file_stream, s);
    free(s);

    //tmpnam_r(file_stream);

    fd_stream = fopen(file_stream, "w");
    fclose(fd_stream);
    fd_stream = fopen(file_stream, "r+");
  }

  static void
  stream_release()
  {
    if (fd_stream != 0)
      {
	fclose(fd_stream);
	remove(file_stream);
      }
  }

  static inline int
  has_zero(const char *buf, int n)
  {
    const char *p = buf;

    while (n--)
      if (!*p++)
	return 1;

    return 0;
  }

  //
  // Warning: this converter is not thread-safe
  //

  ostream& convert_to_stream(ostream &os)
  {
    static char c;
    stream_init();
    fwrite(&c, 1, 1, fd_stream);

    rewind(fd_stream);

    int n;
    char buf[4096];
  
    while ((n = fread(buf, 1, (sizeof buf)-1, fd_stream)) > 0)
      {
	if (isBackendInterrupted()) {
	  setBackendInterrupt(False);
	  return os << "Interrupted!";
	}

	buf[n] = 0;
	os << buf;
	if (has_zero(buf, n))
	  break;
      }

    rewind(fd_stream);

    return os;
  }

  FILE *
  get_file(Bool init)
  {
    stream_init();
    if (init)
      rewind(fd_stream);
    return fd_stream;
  }

  void
  release_all()
  {
    eyedb::release();
  }

  void init()
  {
    static int init;

    if (init)
      return;

    rpc_release_all = release_all;
    init = 1;

    checkLinkedWithMt();

    //stream_init();
    Config::init();
    Architecture::init();
    Class::init();
    Basic::init();
    AgregatClass::init();
    RecMode::init();
    //Connection::init();
    Exception::init();
    Database::init();
    //DBM_Database::init();
    CollectionClass::init();
    ClassConversion::init();
    ThreadPoolManager::init();
    oqml_initialize();

    rpcFeInit();
    eyedbsm::init();
    GenHashTable h(1, 1); // hack
    syscls::init();
    oqlctb::init();
    utils::init();

    DBM_Database::init();

    // WARNING: in the case of eyedb is dynamically loaded in
    // a program (for instance php), one must not call atexit
    atexit(release_all);

    /*
      if (eyedb_probe_name)
      eyedb_probe_h = p_probeInit(eyedb_probe_name);
    */

    ios::sync_with_stdio();
  }

  void release()
  {
    static int release;

    if (release)
      return;

    release = 1;

    Config::_release();
    Architecture::_release();
    Class::_release();
    Basic::_release();
    AgregatClass::_release();
    RecMode::_release();
    Connection::_release();
    Exception::_release();
    Database::_release();
    DBM_Database::__release();
    CollectionClass::_release();
    ClassConversion::_release();
    ThreadPoolManager::_release();
    oqml_release();
    rpcFeRelease();
    eyedbsm::release();
    oqlctb::release();
    utils::release();
    syscls::release();
    stream_release();

    /*
      if (eyedb_probe_h)
      p_probeSave(eyedb_probe_h, eyedb_probe_name);
    */

#ifdef SOCKET_PROFILE
    unsigned int read_cnt, read_tm_cnt, write_cnt, byte_read_cnt, byte_write_cnt;

    rpc_getStats(&read_cnt, &read_tm_cnt, &write_cnt,
		 &byte_read_cnt, &byte_write_cnt);

    cout << "RPC read        : " << read_cnt << endl;
    cout << "RPC read_tm     : " << read_tm_cnt << endl;
    cout << "RPC write       : " << write_cnt << endl;
    cout << "RPC bytes read  : " << byte_read_cnt << endl;
    cout << "RPC bytes write : " << byte_write_cnt << endl;
#endif
  }

  void init(int &argc, char *argv[], string *listen, bool purgeargv) {

    init();

    make_options(argc, argv, 0, 0, listen, purgeargv); 

    Connection::init();

    Status status = Log::init(argv[0], LogName);
    if (status)
      {
	status->print(stderr);
	exit(1);
      }
  }

  extern void IDB_releaseConn();

  void init(int &argc, char *argv[])
  {
    init(argc, argv, 0, true);
  }

  void idbRelease(void)
  {
    DBM_Database::_release();
    IDB_releaseConn();
  }

  const char *
  getVersion()
  {
    return eyedb_version;
  }

  static const char version_fmt[] = "%d.%d.%d";

  const char *
  convertVersionNumber(int version)
  {
#define MAJORCOEF 100000
#define MINORCOEF   1000
    static char s_version[32];
    int major     = version/MAJORCOEF;
    int minor     = (version - major*MAJORCOEF)/MINORCOEF;
    int bug_fixed = (version - major*MAJORCOEF - minor*MINORCOEF);
    sprintf(s_version, version_fmt, major, minor, bug_fixed);
    return s_version;
  }

  unsigned int
  getVersionNumber()
  {
    static unsigned int version;

    if (!version)
      {
	int major, minor, bug_fixed;
	char tok[16];
	sscanf(eyedb_version, version_fmt, &major, &minor, &bug_fixed);
	sprintf(tok, "%d%02d%03d", major, minor, bug_fixed);
	version = atoi(tok);
      }

    return version;
  }
}
