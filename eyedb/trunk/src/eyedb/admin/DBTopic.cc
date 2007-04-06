
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
#include "eyedb/DBM_Database.h"
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>

#include "GetOpt.h"

#include "DBTopic.h"

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

  opts.push_back(Option("dbfile", OptionStringType(), Option::MandatoryValue, OptionDesc("Database file", "<dbfile>")));
  opts.push_back(Option("filedir", OptionStringType(), Option::MandatoryValue, OptionDesc("Database file directory", "<filedir>")));
  opts.push_back(Option("max-object-count", OptionStringType(), Option::MandatoryValue, OptionDesc("Maximum database object count", "<object-count>")));
  getopt = new GetOpt(PROG_NAME, opts);
}

int DBCreateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " <dbname>\n";
  return 1;
}

int DBCreateCmd::help()
{
  getopt->help();
  return 1;
}

int DBCreateCmd::perform(const std::string &prog, const std::vector<std::string> &argv)
{
  if (argv.size() <= 2) {
    return usage();
  }

  return 0;
}
