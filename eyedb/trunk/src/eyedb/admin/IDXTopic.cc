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
// Helper functions
//
static void index_trace(Index *idx, bool full)
{
  if (!full) {
    printf("%s index on %s\n", idx->asHashIndex() ? "hash" : "btree",
	   idx->getAttrpath().c_str());
    return;
  }

  printf("Index on %s:\n", idx->getAttrpath().c_str());
  printf("  Propagation: %s\n", idx->getPropagate() ? "on" : "off");

  const Dataspace *dataspace = 0;
  idx->makeDataspace(db, dataspace);
  if (dataspace)
    printf("  Dataspace: %s #%d\n", dataspace->getName(), dataspace->getId());

  if (idx->asHashIndex()) {
    HashIndex *hidx = idx->asHashIndex();

    printf("  Type: Hash\n");
    printf("  Key count: %d\n", hidx->getKeyCount());
    if (hidx->getHashMethod())
      printf("  Hash method: %s::%s;",
	     hidx->getHashMethod()->getClassOwner()->getName(),
	     hidx->getHashMethod()->getEx()->getExname().c_str());
    int cnt = idx->getImplHintsCount();
    for (int n = 0; n < cnt; n++)
      if (idx->getImplHints(n))
	printf("  %s: %d\n", IndexImpl::hashHintToStr(n, True),
	       idx->getImplHints(n));
  } else {
    BTreeIndex *bidx = idx->asBTreeIndex();
    printf("  Type: BTree\n");
    printf("  Degree: %d\n", bidx->getDegree());
  }
}

static int
get_index( Database *db, const Class *cls, LinkedList &idxlist)
{
  const LinkedList *clidxlist;
  Status s = const_cast<Class *>(cls)->getAttrCompList(Class::Index_C, clidxlist);
  CHECK(s);
    
  LinkedListCursor c(clidxlist);
  void *o;
  while (c.getNext(o)) {
    idxlist.insertObject(o);
  }
  return 0;
}

static int
get_index( Database *db, const char *info, LinkedList &idxlist)
{
  Status s;
  Bool all = False;

  if (info) {
    Status s;
    if (strchr(info, '.')) {
      s = Attribute::checkAttrPath(db->getSchema(), cls, attr, info);
      CHECK(s);
      Index *idx;
      s = Attribute::getIndex(db, info, idx);
      CHECK(s);
      if (idx) {
	idxlist.insertObject(idx);
	return 0;
      }
      print_prog();
      fprintf(stderr, "index '%s' not found\n", info);
      return 1;
    }
    else if (!strcmp(info, "--all"))
      all = True;
    else {
      const Class *cls = db->getSchema()->getClass(info);
      if (!cls) {
	print_prog();
	fprintf(stderr, "class '%s' not found\n", info);
	return 1;
      }
      
      return get_index(cls, idxlist);
    }
  }

  LinkedListCursor c(db->getSchema()->getClassList());
  const Class *cls;
  while (c.getNext((void *&)cls)) {
    if (!cls->isSystem() || all)
      if (get_index(cls, idxlist))
	return 1;
  }
  
  return 0;
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

  const std::string FULL_OPT("full");
  opts.push_back( Option(FULL_OPT, OptionBoolType()));

  getopt = new GetOpt(getExtName(), opts);
}

int IDXListCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME [ATTRPATH|CLASSNAME]...\n";
  return 1;
}

int IDXListCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  getopt->displayOpt("CLASSNAME", "Class name");
  return 1;
}

int IDXListCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();
  if (argv.size() < 1)
    return usage();

  const char *dbname = argv[0].c_str();
  bool full = map.find("full") != map.end();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBSRead);
  CHECK_STATUS(s);

  s = db->transactionBegin();
  CHECK_STATUS(s);

  LinkedList indexList;

  if (argv.size() < 2)
    get_all_index( db, indexList);
  else {
    for ( int i = 1; i < argv.size(); i++)
      get_index( db, argv[i].c_str(), indexList);
  }

  LinkedListCursor c(indexList);
  Index *index;
  while (c.getNext((void *&)index))
    index_trace(index, full);

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
