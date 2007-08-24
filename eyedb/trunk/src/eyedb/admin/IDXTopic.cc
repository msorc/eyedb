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
#include "eyedb/DBM_Database.h"
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>

#include "GetOpt.h"

#include "IDXTopic.h"

IDXTopic::IDXTopic() : Topic("index")
{
  addAlias("idx");

  addCommand(new IDXCreateCmd(this));
  addCommand(new IDXDeleteCmd(this));
  addCommand(new IDXUpdateCmd(this));
  addCommand(new IDXListCmd(this));
  addCommand(new IDXStatsCmd(this));
  addCommand(new IDXSimulateCmd(this));
}

// 
// IDXCreateCmd
//
void IDXCreateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXCreateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME [--check] {ATTRPATH [hash|btree [<hints>|\"\" [propagate=on|propagate=off |\"\"]]]}\n";
  return 1;
}

int IDXCreateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXCreateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}

//
// IDXDeleteCmd
//
void IDXDeleteCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXDeleteCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " delete DBNAME ATTRPATH...\n";
  return 1;
}

int IDXDeleteCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXDeleteCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}

//
// IDXUpdateCmd
//
void IDXUpdateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXUpdateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " update DBNAME [--check] {ATTRPATH [hash|btree [<hints>|\"\"]] [propagate=on|propagate=off |\"\"]}\n";
  return 1;
}

int IDXUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXUpdateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}

//
// IDXListCmd
//
void IDXListCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXListCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME [--full] {[ATTRPATH|<classname>|--all]}\n";
  return 1;
}

int IDXListCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}

//
// IDXStatsCmd
//
void IDXStatsCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXStatsCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " stats DBNAME [--full] [--fmt=<fmt>] {[ATTRPATH|<classname>|--all]}\n";
  return 1;
}

int IDXStatsCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXStatsCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}

//
// IDXSimulateCmd
//
void IDXSimulateCmd::init()
{
  std::vector<Option> opts;
  opts.push_back(HELP_OPT);
  getopt = new GetOpt(getExtName(), opts);
}

int IDXSimulateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " simulate DBNAME [--full] [--fmt=<fmt>] {ATTRPATH hash|btree [<hints>]}\n";
  return 1;
}

int IDXSimulateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  return 1;
}

int IDXSimulateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  return 0;
}
