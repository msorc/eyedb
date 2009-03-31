
/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-2008 SYSRA
   
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
#include "eyedb/DBM_Database.h"
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>
#include "GetOpt.h"
#include "COLTopic.h"

using namespace eyedb;
using namespace std;

COLTopic::COLTopic() : Topic("collection")
{
  addAlias("coll");

  addCommand( new COLUpdateCmd(this));
  addCommand( new COLSimulateCmd(this));
  addCommand( new COLListCmd(this));
  addCommand( new COLStatsCmd(this));
  addCommand( new COLGetDefDSPCmd(this));
  addCommand( new COLSetDefDSPCmd(this));
  addCommand( new COLSetDefImplCmd(this));
  addCommand( new COLGetDefImplCmd(this));
  addCommand( new COLGetLocaCmd(this));
  addCommand( new COLSetLocaCmd(this));
}

//eyedbadmin collection update DATABASE COLLECTION hash|btree [HINTS]
// 
// COLUpdateCmd
//
void COLUpdateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLUpdateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLUpdateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection simulate [--full] [--format=FORMAT] DATABASE COLLECTION hash|btree [HINTS]
// 
// COLSimulateCmd
//
void COLSimulateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLSimulateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLSimulateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLSimulateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection list DATABASE COLLECTION...
// 
// COLListCmd
//
void COLListCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLListCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLListCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection stats [--full] [--format=FORMAT] DATABASE COLLECTION...
// 
// COLStatsCmd
//
void COLStatsCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLStatsCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLStatsCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLStatsCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection getdefdsp DATABASE COLLECTION
// 
// COLGetDefDSPCmd
//
void COLGetDefDSPCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLGetDefDSPCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLGetDefDSPCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLGetDefDSPCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection setdefdsp DATABASE COLLECTION DATASPACE
// 
// COLSetDefDSPCmd
//
void COLSetDefDSPCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLSetDefDSPCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLSetDefDSPCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLSetDefDSPCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection setdefimpl DATABASE ATTRIBUTE_PATH hash|btree [HINTS] [propagate=on|off]
// 
// COLSetDefImplCmd
//
void COLSetDefImplCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLSetDefImplCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLSetDefImplCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLSetDefImplCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection getdefimpl DATABASE [CLASSNAME|ATTRIBUTE_PATH]...
// 
// COLGetDefImplCmd
//
void COLGetDefImplCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLGetDefImplCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLGetDefImplCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLGetDefImplCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

//eyedbadmin collection getloca [--stats] [--loca] DATABASE COLLECTION
// 
// COLGetLocaCmd
//
void COLGetLocaCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLGetLocaCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLGetLocaCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLGetLocaCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

// eyedbadmin collection setloca [--stats] [--loca] DATABASE COLLECTION DATASPACE
// 
// COLSetLocaCmd
//
void COLSetLocaCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int COLSetLocaCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int COLSetLocaCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int COLSetLocaCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}

#if 0
// 
// XXXTemplateCmd
//
void XXXTemplateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int XXXTemplateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ...\n";
  return 1;
}

int XXXTemplateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  return 1;
}

int XXXTemplateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROGNAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbName = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  // stuff here...

  db->transactionCommit();

  return 0;
}
#endif
