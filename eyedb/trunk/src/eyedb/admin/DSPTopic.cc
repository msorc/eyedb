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

#include "eyedbconfig.h"
#include <eyedb/eyedb.h>
#include <eyedb/opts.h>
#include "eyedb/DBM_Database.h"
#include "GetOpt.h"
#include "DSPTopic.h"

using namespace eyedb;
using namespace std;

#define CHECK_STATUS(s) 			\
  if (s) {					\
    std::cerr << PROG_NAME;			\
    s->print();					\
    return 1;					\
  }

DSPTopic::DSPTopic() : Topic("dataspace")
{
  addAlias("dsp");

  addCommand(new DSPCreateCmd(this));
  addCommand(new DSPUpdateCmd(this));
  addCommand(new DSPDeleteCmd(this));
  addCommand(new DSPRenameCmd(this));
  addCommand(new DSPListCmd(this));
  addCommand(new DSPSetDefCmd(this));
  addCommand(new DSPGetDefCmd(this));
  addCommand(new DSPSetCurDatCmd(this));
  addCommand(new DSPGetCurDatCmd(this));
}

// DSPCreateCmd 
void DSPCreateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPCreateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname> {<datid>|<datname>}\n";
  return 1;
}

int DSPCreateCmd::help()
{
  stdhelp();
  getopt->displayOpt("<dbname>", "Data base name");
  getopt->displayOpt("<dspname>", "Data space name");
  getopt->displayOpt("<datid>", "Data file id");
  getopt->displayOpt("<datname>", "Data file name");
  return 1;
}

int DSPCreateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPUpdateCmd 
void DSPUpdateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPUpdateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname> {<datid>|<datname>}\n";
  return 1;
}

int DSPUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPUpdateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPDeleteCmd 
void DSPDeleteCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPDeleteCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname>\n";
  return 1;
}

int DSPDeleteCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPDeleteCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPRenameCmd 
void DSPRenameCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPRenameCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " ?\n";
  return 1;
}

int DSPRenameCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPRenameCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPListCmd 
void DSPListCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPListCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> [{<dspname>}]\n";
  return 1;
}

int DSPListCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPSetDefCmd 
void DSPSetDefCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPSetDefCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname>\n";
  return 1;
}

int DSPSetDefCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPSetDefCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPGetDefCmd 
void DSPGetDefCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPGetDefCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname>\n";
  return 1;
}

int DSPGetDefCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPGetDefCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPSetCurDatCmd 
void DSPSetCurDatCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPSetCurDatCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname> <datid>|<datname>\n";
  return 1;
}

int DSPSetCurDatCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPSetCurDatCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}

// DSPGetCurDatCmd 
void DSPGetCurDatCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int DSPGetCurDatCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname> <dspname>\n";
  return 1;
}

int DSPGetCurDatCmd::help()
{
  stdhelp();
  getopt->displayOpt("<user>", "User name");
  getopt->displayOpt("<passwd>", "Password for specified user");
  return 1;
}

int DSPGetCurDatCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  std::cerr << " not yet implemented\n";
  return 1;

  //  DBM_Database *dbmdatabase = new DBM_Database();

  //  conn.open();

  //  Status s = dbmdatabase->addUser(&conn, username, passwd, user_type, userauth, passwdauth);

  //  CHECK_STATUS(s);

  return 0;
}


