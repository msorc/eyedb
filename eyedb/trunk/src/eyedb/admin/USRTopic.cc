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
   Author: Francois Dechelle <francois@dechelle.net>
*/

#include "eyedbconfig.h"

#include <eyedb/eyedb.h>
#include <eyedb/opts.h>
#include "eyedb/DBM_Database.h"

#include "GetOpt.h"
#include "USRTopic.h"

#include <errno.h>

using namespace eyedb;
using namespace std;

USRTopic::USRTopic() : Topic("user")
{
  addAlias("usr");

  addCommand(new USRAddCmd(this));
  addCommand(new USRDeleteCmd(this));
  addCommand(new USRListCmd(this));
  addCommand(new USRSysAccessCmd(this));
  addCommand(new USRDBAccessCmd(this));
  addCommand(new USRPasswdCmd(this));
}


//
// Helper functions
//

#define N 4

static void
passwd_realize(const char *prompt, char **passwd, int retype = 1)
{
  static int n;
  static char pswd[N][12];
  char *p, *q, buf[128];

  for (;;)
    {
      errno = 0;
      p = pswd[(n >= N ? (n = 0) : 0), n++];

      sprintf(buf, "%s: ", prompt);
      strcpy(p, getpass(buf));
      if (errno)
	exit(1);

      if (!retype)
	break;

      q = pswd[(n >= N ? (n = 0) : 0), n++];

      sprintf(buf, "retype %s: ", prompt);
      strcpy(q, getpass(buf));
      if (errno)
	exit(1);
      if (!strcmp(q, p))
	break;
      printf("passwords differ, try again.\n");
    }

  *passwd = p;
}

static void
auth_realize( char *userauth, char *passwdauth)
{
  const char *s;
  errno = 0;

  if (!(s = Connection::getDefaultUser()))
    *userauth = 0;
  else
    strcpy(userauth, s);

  if (!(s = Connection::getDefaultPasswd()))
    *passwdauth = 0;
  else
    strcpy(passwdauth, s);
}

//
// Commands
//

//
// eyedbadmin user add ...
//	    "%suseradd [--unix|--strict-unix] [<user>|@ [<passwd>]]\n", str());
//
static const string ADD_UNIX_OPT("unix");
static const string ADD_STRICT_UNIX_OPT("strict-unix");

void USRAddCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);
  opts.push_back( Option(ADD_UNIX_OPT,
			 OptionBoolType()));
  opts.push_back( Option(ADD_STRICT_UNIX_OPT, 
			 OptionBoolType()));

  getopt = new GetOpt(getExtName(), opts);
}

int USRAddCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " [<user> [<passwd>]]\n";
  return 1;
}

int USRAddCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int USRAddCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  if (!r) {
    return usage();
  }

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  UserType user_type;

  if (map.find(ADD_UNIX_OPT) != map.end()) {
    user_type = UnixUser;
  } else if (map.find(ADD_STRICT_UNIX_OPT) != map.end()) {
    user_type = StrictUnixUser;
  } else
    user_type = EyeDBUser;

  char *username = 0;
  char *passwd = 0;

  if (argv.size() >= 1) {
    username = strdup(argv[0].c_str());

    if (argv.size() >= 2)
      passwd = strdup(argv[1].c_str());
  }

  if (!username && user_type == EyeDBUser)
    return help();

  if (!username)
    username = "@";

  std::string user = Connection::makeUser(username);
  username = strdup(user.c_str());

  if (!passwd && user_type != StrictUnixUser) {
    char buf[128];

    sprintf(buf, "%s password", username);
    passwd_realize(buf, &passwd);
  }
  if (!passwd)
    passwd = "";

  char userauth[32];
  char passwdauth[10];

  auth_realize( userauth, passwdauth);

  DBM_Database *dbmdatabase = new DBM_Database();

  conn.open();

  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  if (s) {
    std::cerr << PROG_NAME;
    s->print();
    return 1;
  }

  return 0;
}



//
// eyedbadmin user delete ...
//

void USRDeleteCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  getopt = new GetOpt(getExtName(), opts);
}

int USRDeleteCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <user>\n";
  return 1;
}

int USRDeleteCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  return 1;
}

int USRDeleteCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  if (!r) {
    return usage();
  }

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (argv.size() < 1) {
    return usage();
  }

  char *username = strdup(argv[0].c_str());

  std::string user = Connection::makeUser(username);
  username = strdup(user.c_str());

  char userauth[32];
  char passwdauth[10];

  auth_realize( userauth, passwdauth);

  DBM_Database *dbmdatabase = new DBM_Database();

  conn.open();

  Status s = dbmdatabase->deleteUser( &conn, username, userauth, passwdauth);

  if (s) {
    std::cerr << PROG_NAME;
    s->print();
    return 1;
  }

  return 0;
}


//
// eyedbadmin user list ...
//

void USRListCmd::init()
{
}

int USRListCmd::usage()
{
  std::cerr << " not yet implemented\n";
  return 1;
}

int USRListCmd::help()
{
  std::cerr << " not yet implemented\n";
  stdhelp();
  return 1;
}

int USRListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  std::cerr << " not yet implemented\n";
  return 0;
}


//
// eyedbadmin user sysaccess ...
//

void USRSysAccessCmd::init()
{
}

int USRSysAccessCmd::usage()
{
  std::cerr << " not yet implemented\n";
  return 1;
}

int USRSysAccessCmd::help()
{
  std::cerr << " not yet implemented\n";
  stdhelp();
  return 1;
}

int USRSysAccessCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  std::cerr << " not yet implemented\n";
  return 0;
}


// 
// eyedbadmin user dbaccess ...
//

void USRDBAccessCmd::init()
{
}

int USRDBAccessCmd::usage()
{
  std::cerr << " not yet implemented\n";
  return 1;
}

int USRDBAccessCmd::help()
{
  std::cerr << " not yet implemented\n";
  stdhelp();
  return 1;
}

int USRDBAccessCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  std::cerr << " not yet implemented\n";
  return 0;
}

//
// eyedbadmin user passwd ...
//

void USRPasswdCmd::init()
{
}

int USRPasswdCmd::usage()
{
  std::cerr << " not yet implemented\n";
  return 1;
}

int USRPasswdCmd::help()
{
  std::cerr << " not yet implemented\n";
  stdhelp();
  return 1;
}

int USRPasswdCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  std::cerr << " not yet implemented\n";
  return 0;
}

