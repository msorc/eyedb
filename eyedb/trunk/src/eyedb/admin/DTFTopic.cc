
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

#include "DTFTopic.h"

using namespace eyedb;

DTFTopic::DTFTopic() : Topic("datafile")
{
  addAlias("dtf");

  addCommand(new DTFCreateCmd(this));
  addCommand(new DTFDeleteCmd(this));
  addCommand(new DTFMoveCmd(this));
  addCommand(new DTFResizeCmd(this));
  addCommand(new DTFDefragmentCmd(this));
  addCommand(new DTFListCmd(this));
  addCommand(new DTFRenameCmd(this));
}

void DTFCreateCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  opts.push_back(Option(FILENAME_OPT, OptionStringType(), Option::MandatoryValue, OptionDesc("File name", "<filename>")));

  opts.push_back(Option(FILEDIR_OPT, OptionStringType(), Option::MandatoryValue, OptionDesc("File directory", "<filedir>")));

  opts.push_back(Option(NAME_OPT, OptionStringType(), Option::MandatoryValue, OptionDesc("Datafile name", "<name>")));

  opts.push_back(Option(SIZE_OPT, OptionStringType(), Option::MandatoryValue, OptionDesc("Datafile size (in mega-bytes)", "<size>")));

  opts.push_back(Option(SLOTSIZE_OPT, OptionStringType(), Option::MandatoryValue, OptionDesc("Slot size (in bytes)", "<slotsize>")));

  opts.push_back(Option(PHYSICAL_OPT, OptionBoolType(), 0, OptionDesc("Physical datafile type")));

  getopt = new GetOpt(getExtName(), opts);
}

int DTFCreateCmd::usage()
{
  getopt->usage("", "");

  std::cerr << "<dbname>\n";

  return 1;
}

int DTFCreateCmd::help()
{
  stdhelp();

  getopt->displayOpt("<dbname>", "Database");

  return 1;
}

int DTFCreateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  if (argv.size() != 1) {
    return usage();
  }

  conn.open();

  std::string dbname = argv[0];

  Database *db = new Database(dbname.c_str());

  const char *filedir;
  if (map.find(FILEDIR_OPT) != map.end()) {
    filedir = map[FILEDIR_OPT].value.c_str();
  }
  else {
    filedir = 0;
  }

  const char *filename;
  if (map.find(FILENAME_OPT) != map.end()) {
    filename = map[FILENAME_OPT].value.c_str();
  }
  else {
    filename = 0;
  }

  const char *name;
  if (map.find(NAME_OPT) != map.end()) {
    name = map[NAME_OPT].value.c_str();
  }
  else {
    name = 0;
  }

  unsigned int size;
  if (map.find(SIZE_OPT) != map.end()) {
    size = atoi(map[SIZE_OPT].value.c_str());
  }
  else {
    size = DEFAULT_DTFSIZE * ONE_K;
  }

  unsigned int slotsize;
  if (map.find(SLOTSIZE_OPT) != map.end()) {
    slotsize = atoi(map[SLOTSIZE_OPT].value.c_str());
  }
  else {
    slotsize = DEFAULT_DTFSZSLOT;
  }

  eyedbsm::DatType dtfType;
  if (map.find(PHYSICAL_OPT) != map.end()) {
    dtfType = eyedbsm::PhysicalOidType;
  }
  else {
    dtfType = eyedbsm::LogicalOidType;
  }

  db->createDatafile(filedir, filename, name, size, slotsize, dtfType);

  return 0;
}

void DTFDeleteCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  // [...]

  getopt = new GetOpt(getExtName(), opts);
}

int DTFDeleteCmd::usage()
{
  getopt->usage("", "");

  // std::cerr << "Extra arg\n";

  return 1;
}

int DTFDeleteCmd::help()
{
  stdhelp();

  //getopt->displayOpt("Extra arg, "Description");

  return 1;
}

int DTFDeleteCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  /*
  if (argv.size() != N) { // N : number of extra args (could be 0)
    return usage();
  }
  */

  conn.open();

  // [...]

  return 0;
}

void DTFListCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  // [...]

  getopt = new GetOpt(getExtName(), opts);
}

int DTFListCmd::usage()
{
  getopt->usage("", "");

  // std::cerr << "Extra arg\n";

  return 1;
}

int DTFListCmd::help()
{
  stdhelp();

  //getopt->displayOpt("Extra arg, "Description");

  return 1;
}

int DTFListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  /*
  if (argv.size() != N) { // N : number of extra args (could be 0)
    return usage();
  }
  */

  conn.open();

  // [...]

  return 0;
}

