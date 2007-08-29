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
#include <eyedblib/butils.h>
#include "eyedb/DBM_Database.h"
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>
#include "GetOpt.h"
#include "IDXTopic.h"

using namespace eyedb;
using namespace std;

#define CHECK_STATUS(s) 			\
  if (s) {					\
    std::cerr << PROG_NAME;			\
    s->print();					\
    return 1;					\
  }

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

static const std::string PROPAGATE_OPT("propagate");
static const std::string TYPE_OPT("type");

//
// Helper functions
//
static void index_trace( Database *db, Index *idx, bool full)
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
index_get( Database *db, const Class *cls, LinkedList &indexlist)
{
  const LinkedList *classindexlist;

  Status s = const_cast<Class *>(cls)->getAttrCompList(Class::Index_C, classindexlist);
  CHECK_STATUS(s);
    
  LinkedListCursor c(classindexlist);
  void *o;
  while (c.getNext(o)) {
    indexlist.insertObject(o);
  }

  return 0;
}

static int
index_get( Database *db, const char *name, LinkedList &indexlist)
{
  Status s;
  const Class *cls;
  const Attribute *attr;

  if (strchr(name, '.')) {
    // name is an attribute name
    s = Attribute::checkAttrPath(db->getSchema(), cls, attr, name);
    CHECK_STATUS(s);

    Index *index;
    s = Attribute::getIndex(db, name, index);
    CHECK_STATUS(s);

    if (!index) {
      std::cerr << PROG_NAME;
      fprintf(stderr, ": index '%s' not found\n", name);
      return 1;
    }

    indexlist.insertObject(index);

    return 0;
  } else {
    // name is a class name
    cls = db->getSchema()->getClass(name);
    if (!cls) {
      std::cerr << PROG_NAME;
      fprintf(stderr, ": class '%s' not found\n", name);
      return 1;
    }
      
    return index_get(db, cls, indexlist);
  }
  
  return 0;
}

static int
index_get_all( Database *db, LinkedList &indexlist, bool all)
{
  LinkedListCursor c(db->getSchema()->getClassList());
  const Class *cls;
  while (c.getNext((void *&)cls)) {
    if (!cls->isSystem() || all)
      if (index_get(db, cls, indexlist))
	return 1;
  }
  
  return 0;
}

// 
// IDXCreateCmd
//
static void make_create_update_options( std::vector<Option> &opts)
{
  opts.push_back(HELP_OPT);

  std::vector<std::string> propagate_choices;
  const std::string ON("on");
  const std::string OFF("off");
  propagate_choices.push_back( ON);
  propagate_choices.push_back( OFF);
  opts.push_back( Option(PROPAGATE_OPT, 
			 OptionChoiceType("on_off",propagate_choices,ON),
			 Option::MandatoryValue,
			 OptionDesc( "Propagation type", "on|off")));

  opts.push_back( Option(TYPE_OPT, 
			 OptionStringType(),
			 Option::MandatoryValue,
			 OptionDesc( "Index type (supported types are: hash, btree)", "TYPE")));
}

void IDXCreateCmd::init()
{
  std::vector<Option> opts;

  make_create_update_options( opts);

  getopt = new GetOpt(getExtName(), opts);
}

int IDXCreateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ATTRPATH [HINTS]\n";
  return 1;
}

int IDXCreateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  getopt->displayOpt("HINTS", "Index hints");
  return 1;
}

int IDXCreateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *attributePath = argv[1].c_str();

  const char *hints = 0;
  if (argv.size() > 2)
    hints = argv[2].c_str();

  const char *typeOption = 0;
  if (map.find(TYPE_OPT) != map.end()) {
    typeOption = map[TYPE_OPT].value.c_str();

    if (strcmp(typeOption, "hash") && strcmp(typeOption, "btree"))
      return help();
  }

  bool propagate = true;
  if (map.find(PROPAGATE_OPT) != map.end()) {
    propagate = !strcmp( map[PROPAGATE_OPT].value.c_str(), "on");
  }

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Class *cls;
  const Attribute *attribute;

  s = Attribute::checkAttrPath( db->getSchema(), cls, attribute, attributePath);
  CHECK_STATUS(s);

  if (!typeOption) {
    if (attribute->isString() 
	|| attribute->isIndirect() 
	|| attribute->getClass()->asCollectionClass()) // what about enums
      typeOption = "hash";
    else
      typeOption = "btree";
  }

  Index *index;

  printf("Creating %s index on %s\n", typeOption, attributePath);

  if (!strcmp(typeOption, "hash")) {
    HashIndex *hidx;
    Status s = HashIndex::make(db, const_cast<Class *>(cls), attributePath,
			       (propagate)? eyedb::True : eyedb::False, 
			       attribute->isString(), hints, hidx);
    CHECK_STATUS(s);
    index = hidx;
  }
  else if (!strcmp(typeOption, "btree")) {
    BTreeIndex *bidx;
    Status s = BTreeIndex::make(db, const_cast<Class *>(cls), attributePath, 
			       (propagate)? eyedb::True : eyedb::False, 
				attribute->isString(), hints, bidx);
    CHECK_STATUS(s);
    index = bidx;
  }

  s = index->store();
  CHECK_STATUS(s);

  db->transactionCommit();

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

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *attributePath = argv[1].c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  const Class *cls;
  const Attribute *attribute;

  s = Attribute::checkAttrPath(db->getSchema(), cls, attribute, attributePath);
  CHECK_STATUS(s);

  Index *index;

  s = Attribute::getIndex(db, attributePath, index);
  CHECK_STATUS(s);
    
  if (!index) {
    std::cerr << PROG_NAME;
    fprintf(stderr, ": index '%s' not found\n", attributePath);
    return 1;
  }

  printf("Deleting index %s\n", index->getAttrpath().c_str());
  s = index->remove();
  CHECK_STATUS(s);
    
  db->transactionCommit();

  return 0;
}

//
// IDXUpdateCmd
//
void IDXUpdateCmd::init()
{
  std::vector<Option> opts;

  make_create_update_options( opts);

  getopt = new GetOpt(getExtName(), opts);
}

int IDXUpdateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME ATTRPATH [HINTS]\n";
  return 1;
}

int IDXUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  getopt->displayOpt("HINTS", "Index hints");
  return 1;
}

int IDXUpdateCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  if (! getopt->parse(PROG_NAME, argv))
    return usage();

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end())
    return help();

  if (argv.size() < 2)
    return usage();

  const char *dbname = argv[0].c_str();
  const char *attributePath = argv[1].c_str();

  const char *hints = 0;
  if (argv.size() > 2)
    hints = argv[2].c_str();

  const char *typeOption = 0;
  if (map.find(TYPE_OPT) != map.end()) {
    typeOption = map[TYPE_OPT].value.c_str();

    if (strcmp(typeOption, "hash") && strcmp(typeOption, "btree"))
      return help();
  }

  bool propagate = true;
  if (map.find(PROPAGATE_OPT) != map.end()) {
    propagate = !strcmp( map[PROPAGATE_OPT].value.c_str(), "on");
  }

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBRW);
  CHECK_STATUS(s);

  s = db->transactionBeginExclusive();
  CHECK_STATUS(s);

  LinkedList indexList;
  if (index_get( db, attributePath, indexList))
    return 1;

  Index *index = (Index *)indexList.getObject(0);

  IndexImpl::Type type;
  if (typeOption && !strcmp(typeOption, "hash"))
    type = IndexImpl::Hash;
  else if (typeOption && !strcmp(typeOption, "btree"))
    type = IndexImpl::BTree;
  else
    type = index->asBTreeIndex() ? IndexImpl::BTree : IndexImpl::Hash;

#if 0
  Bool propag;
  Bool only_propag = False;
  const char *stype = argv[n+1];

  if (!strcmp(stype, "hash"))
    type = IndexImpl::Hash;
  else if (!strcmp(stype, "btree"))
    type = IndexImpl::BTree;
  else if (!*stype)
    type = idx->asBTreeIndex() ? IndexImpl::BTree : IndexImpl::Hash;
  else if (!get_propag(stype, propag)) {
    only_propag = True;
    stype = idx->asBTreeIndex() ? "btree" : "hash";
  }
  else
    return usage(prog);
  
  IndexImpl *idximpl = 0;
  Status s;
  if (!only_propag) {
    const char *hints = (argc > n+2 ? argv[n+2] : "");
    s = IndexImpl::make(db, type, hints, idximpl, idx->getIsString());
    CHECK(s);

    if (argc > n+3) {
      if (get_propag(argv[n+3], propag))
	return 1;
      else
	propag = idx->getPropagate();
    }
  }

  printf("Updating %sindex on %s\n", stype, argv[n]);
  idx->setPropagate(propag);
  if (only_propag) {
    s = idx->store();
    CHECK(s);
  }
  else if ((type == IndexImpl::Hash && idx->asBTreeIndex()) ||
	   (type == IndexImpl::BTree && idx->asHashIndex())) {
    Index *idxn;
    s = idx->reimplement(*idximpl, idxn);
    CHECK(s);
  }
  else {
    s = idx->setImplementation(idximpl);
    CHECK(s);
    s = idx->store();
    CHECK(s);
  }
#endif

  db->transactionCommit();

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

  const std::string ALL_OPT("all");
  opts.push_back( Option(ALL_OPT, OptionBoolType()));

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
  bool all = map.find("all") != map.end();

  if (all && argv.size() > 1)
    return usage();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBSRead);
  CHECK_STATUS(s);

  s = db->transactionBegin();
  CHECK_STATUS(s);

  LinkedList indexList;

  if (argv.size() < 2) {
    if (index_get_all( db, indexList, all))
      return 1;
  } else {
    for ( int i = 1; i < argv.size(); i++) {
      if (index_get( db, argv[i].c_str(), indexList))
	return 1;
    }
  }

  LinkedListCursor c(indexList);
  Index *index;
  while (c.getNext((void *&)index))
    index_trace( db, index, full);

  return 0;
}

//
// IDXStatsCmd
//
void IDXStatsCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  const std::string FULL_OPT("full");
  opts.push_back( Option(FULL_OPT, OptionBoolType()));

  const std::string FORMAT_OPT("format");
  opts.push_back( Option(FORMAT_OPT, 
			 OptionStringType(),
			 Option::MandatoryValue,
			 OptionDesc( "Statistics format", "FORMAT")));

  getopt = new GetOpt(getExtName(), opts);
}

int IDXStatsCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME [ATTRPATH|CLASSNAME]...\n";
  return 1;
}

int IDXStatsCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Data base name");
  getopt->displayOpt("ATTRPATH", "Attribute path");
  getopt->displayOpt("CLASSNAME", "Class name");
  std::cout << 
"\n  The --format option indicates an output format for hash index stat entries.\n\
    <format> is a printf-like string where:\n\
      %n denotes the number of keys,\n\
      %O denotes the count of object entries for this key,\n\
      %o denotes the count of hash objects for this key,\n\
      %s denotes the size of hash objects for this key,\n\
      %b denotes the busy size of hash objects for this key,\n\
      %f denotes the free size of hash objects for this key.\n\
\n\
  Examples:\n\
      --format=\"%n %O\\n\"\n\
      --format=\"%n -> %O, %o, %s\\n\"\n";

  return 1;
}

int IDXStatsCmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
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

  const char *format = 0;
  if (map.find("format") != map.end())
    format = map["format"].value.c_str();

  conn.open();

  Database *db = new Database(dbname);

  Status s = db->open( &conn, Database::DBSRead);
  CHECK_STATUS(s);

  s = db->transactionBegin();
  CHECK_STATUS(s);

  LinkedList indexList;

  if (argv.size() < 2) {
    if (index_get_all( db, indexList, false))
      return 1;
  } else {
    for ( int i = 1; i < argv.size(); i++) {
      if (index_get( db, argv[i].c_str(), indexList))
	return 1;
    }
  }

  LinkedListCursor c(indexList);
  Index *index;
  while (c.getNext((void *&)index)) {
    if (format && index->asHashIndex()) {
      IndexStats *stats;
      s = index->getStats(stats);
      CHECK_STATUS(s);
      fprintf(stdout, "\n");
      s = stats->asHashIndexStats()->printEntries(format);
      delete stats;
      CHECK_STATUS(s);
    } else {
      std::string stats;
      s = index->getStats(stats, False, (full)? eyedb::True : eyedb::False, "    ");
      CHECK_STATUS(s);
      index_trace(db, index, True);
      fprintf(stdout, "  Statistics:\n");
      fprintf(stdout, stats.c_str());
    }
  }

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
