
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
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>

#include "GetOpt.h"

#include "DBTopic.h"

using namespace eyedb;

DBTopic::DBTopic() : Topic("database")
{
  addAlias("db");

  addCommand(new DBDBMCreateCmd(this));

  addCommand(new DBCreateCmd(this));
  addCommand(new DBDeleteCmd(this));
  addCommand(new DBListCmd(this));

  addCommand(new DBMoveCmd(this));
  addCommand(new DBCopyCmd(this));
  addCommand(new DBRenameCmd(this));

  addCommand(new DBAccessCmd(this));

  addCommand(new DBExportCmd(this));
  addCommand(new DBImportCmd(this));
}

void DBCreateCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(Option("dbfile", OptionStringType(), /*Option::Mandatory|*/Option::MandatoryValue, OptionDesc("Database file", "<dbfile>")));
  opts.push_back(Option("filedir", OptionStringType(), Option::MandatoryValue, OptionDesc("Database file directory", "<filedir>")));
  opts.push_back(Option("max-object-count", OptionStringType(), Option::MandatoryValue, OptionDesc("Maximum database object count", "<object-count>")));
  opts.push_back(HELP_OPT);
  opts.push_back(HELP_COMMON_OPT);

  getopt = new GetOpt(getExtName(), opts);
}

int DBCreateCmd::usage()
{
  getopt->usage("");
  std::cerr << " <dbname>\n";
  return 1;
}

int DBCreateCmd::help()
{
  stdhelp();
  getopt->displayOpt("<dbname>", "Database to create");
  return 1;
}

int DBCreateCmd::perform(eyedb::Connection &conn, const std::string &prog, std::vector<std::string> &argv)
{
  bool r = getopt->parse(prog, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  if (argv.size() != 1) { // dbname is missing
    return usage();
  }

  printf("performing...\n");

  return 0;
}

void DBDeleteCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);
  opts.push_back(HELP_COMMON_OPT);

  getopt = new GetOpt(getExtName(), opts);
}

int DBDeleteCmd::usage()
{
  getopt->usage("");
  std::cerr << " {<dbname>}\n";
  return 1;
}

int DBDeleteCmd::help()
{
  stdhelp();
  getopt->displayOpt("{<dbname>}", "Database(s) to delete");
  return 1;
}

int DBDeleteCmd::perform(eyedb::Connection &conn, const std::string &prog, std::vector<std::string> &argv)
{
  bool r = getopt->parse(prog, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  if (argv.size() < 1) { // dbname is missing
    return usage();
  }

  bool error = false;

  for (unsigned int n = 0; n < argv.size(); n++) {
    try {
      eyedb::Database db(&conn, argv[n].c_str());
      const char *dbfile;
      db.getDatabasefile(dbfile);
    }
    catch(Exception &e) {
      std::cerr << e << std::endl;
      error = true;
    }
  }

  if (error)
    return 1;

  for (unsigned int n = 0; n < argv.size(); n++) {
    eyedb::Database db(argv[n].c_str());
    db.remove(&conn);
  }

  return 0;
}
