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
  std::cerr << " DBNAME DSPNAME DATID|DATNAME...\n";
  return 1;
}

int DSPCreateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  getopt->displayOpt("DATID", "Data file id");
  getopt->displayOpt("DATNAME", "Data file name");
  return 1;
}

int DSPCreateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 3)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  int count = argv.size() - 2;
  const Datafile **datafiles = new const Datafile *[count];
  for ( int i = 0; i < count; i++) {
    s = db->getDatafile( argv[i+2].c_str(), datafiles[i]);
    CHECK_STATUS(s);
  }

  s = db->createDataspace( dspname, datafiles, count);
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME DSPNAME DATID|DATNAME...\n";
  return 1;
}

int DSPUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  getopt->displayOpt("DATID", "Data file id");
  getopt->displayOpt("DATNAME", "Data file name");
  return 1;
}

int DSPUpdateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 3)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace = 0;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  int count = argv.size() - 2;
  const Datafile **datafiles = new const Datafile *[count];
  for (int i = 0; i < count; i++) {
    s = db->getDatafile( argv[i+2].c_str(), datafiles[i]);
    CHECK_STATUS(s);
  }

  s = dataspace->update(datafiles, count);
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME DSPNAME\n";
  return 1;
}

int DSPDeleteCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  return 1;
}

int DSPDeleteCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace = 0;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  s = dataspace->remove();
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME DSPNAME NEWDSPNAME\n";
  return 1;
}

int DSPRenameCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  getopt->displayOpt("NEWDSPNAME", "New data space name");
  return 1;
}

int DSPRenameCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 3)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();
  const char *newdspname = argv[2].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace = 0;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  s = dataspace->rename(newdspname);
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME [DSPNAME]...\n";
  return 1;
}

int DSPListCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  return 1;
}

int DSPListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 1)
    return usage();

  const char *dbname = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBSRead);
  CHECK_STATUS(s);

  if ( argv.size() > 1) {
    for (int i = 1; i < argv.size(); i++) {
      const Dataspace *dataspace;
      s = db->getDataspace( argv[i].c_str(), dataspace);
      CHECK_STATUS(s);
      cout << *dataspace;
    }
  }
  else {
    unsigned int count;
    const Dataspace **dataspaces;
    s = db->getDataspaces(dataspaces, count);
    CHECK_STATUS(s);
    for (unsigned int i = 0; i < count; i++) {
      if (dataspaces[i]->isValid()) {
	cout << *dataspaces[i];
      }
    }
  }

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
  std::cerr << " DBNAME DSPNAME\n";
  return 1;
}

int DSPSetDefCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  return 1;
}

int DSPSetDefCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace = 0;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  s = db->setDefaultDataspace(dataspace);
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME\n";
  return 1;
}

int DSPGetDefCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  return 1;
}

int DSPGetDefCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 1)
    return usage();

  const char *dbname = argv[0].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace;
  s = db->getDefaultDataspace(dataspace);
  CHECK_STATUS(s);
  cout << *dataspace;

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
  std::cerr << " DBNAME DSPNAME DATID|DATNAME\n";
  return 1;
}

int DSPSetCurDatCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  getopt->displayOpt("DATID", "Data file id");
  getopt->displayOpt("DATNAME", "Data file name");
  return 1;
}

int DSPSetCurDatCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 3)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();
  const char *datname = argv[2].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  const Datafile *datafile;
  s = db->getDatafile(datname, datafile);
  CHECK_STATUS(s);

  s = const_cast<Dataspace *>(dataspace)->setCurrentDatafile(datafile);
  CHECK_STATUS(s);

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
  std::cerr << " DBNAME DSPNAME\n";
  return 1;
}

int DSPGetCurDatCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("DSPNAME", "Data space name");
  return 1;
}

int DSPGetCurDatCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *dspname = argv[1].c_str();


  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Dataspace *dataspace;
  s = db->getDataspace(dspname, dataspace);
  CHECK_STATUS(s);

  const Datafile *datafile;
  s = dataspace->getCurrentDatafile(datafile);
  CHECK_STATUS(s);

  cout << *datafile;

  return 0;
}


