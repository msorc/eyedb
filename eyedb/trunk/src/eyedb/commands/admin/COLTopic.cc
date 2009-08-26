
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


// Global variables...

static Connection *conn;
static Database *db;
static const Attribute *attr;
static const Class *cls;

//
// Helper functions and macros
//

static int
get_collimpl(const char *attrpath, CollAttrImpl *&collimpl)
{
  Status s = Attribute::checkAttrPath(db->getSchema(), cls, attr, attrpath);
  s = Attribute::getCollAttrImpl(db, attrpath, collimpl);
  if (!attr->getClass()->asCollectionClass() || attr->isIndirect()) {
    fprintf(stderr, "attribute path %s: "
	    "a collection implementation can be tied "
	    "only to a literal collection attribute\n",
	    attrpath);
    return 1;
  }
  return 0;
}

static int
get_collimpls(const Class *cls, LinkedList &list)
{
  const LinkedList *cllist;
  Status s = const_cast<Class *>(cls)->getAttrCompList(Class::CollectionImpl_C, cllist);
    
  LinkedListCursor c(cllist);
  void *o;
  while (c.getNext(o))
    list.insertObject(o);
  return 0;
}

static int
get_collimpls(const char *info, LinkedList &list)
{
  Status s;
  if (info) {
    Status s;
    if (strchr(info, '.')) {
      s = Attribute::checkAttrPath(db->getSchema(), cls, attr, info);
      CollAttrImpl *collimpl;
      if (get_collimpl(info, collimpl))
	return 1;
      if (collimpl) {
	list.insertObject(collimpl);
	return 0;
      }
      return 0;
    }
    
    const Class *cls = db->getSchema()->getClass(info);
    if (!cls) {
      fprintf(stderr, "class '%s' not found\n", info);
      return 1;
    }
    
    return get_collimpls(cls, list);
  }

  LinkedListCursor c(db->getSchema()->getClassList());
  const Class *cls;
  while (c.getNext((void *&)cls)) {
    if (get_collimpls(cls, list))
      return 1;
  }
  
  return 0;
}
static int
get_collections(const char *str, LinkedList &list)
{
  Status s;
  std::string query;

  int len = strlen(str);
  if (len > 4 && !strcmp(&str[len-4], ":oid")) {
    Oid oid(str);
    Object *o;
    s = db->loadObject(oid, o);
    list.insertObject(o);
    return 0;
  }

  Bool collname;
  if (len > 4 && !strcmp(&str[len-4], ":oql")) {
    char *x = new char[len-3];
    strncpy(x, str, len-4);
    x[len-4] = 0;
    query = x;
    free(x);
    
    collname = False;
  }
  else {
    query = std::string("select collection.name = \"") + str + "\"";
    collname = True;
  }

  OQL oql(db, query.c_str());
  ObjectArray obj_arr;
  s = oql.execute(obj_arr);
  if (!obj_arr.getCount() && collname) {
    fprintf(stderr, "No collection named '%s'\n", str);
    return 1;
  }

  for (int n = 0; n < obj_arr.getCount(); n++)
    if (obj_arr[n]->asCollection())
      list.insertObject(const_cast<Object *>(obj_arr[n]));
  return 0;
}

static void
collection_trace(Collection *coll)
{
  fprintf(stdout, "Collection %s:\n", coll->getOid().toString());
  fprintf(stdout, "  Name: '%s'\n", coll->getName());
  fprintf(stdout, "  Collection of: %s *\n", coll->getClass()->asCollectionClass()->getCollClass()->getName());
}


//
// Topic definition
//

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

static const std::string FULL_OPT("full");
static const std::string FORMAT_OPT("format");
static const std::string TYPE_OPT("type");

//eyedbadmin collection update DATABASE COLLECTION hash|btree [HINTS]
// 
// COLUpdateCmd
//
void COLUpdateCmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  opts.push_back(Option(TYPE_OPT, 
			OptionStringType(),
			Option::MandatoryValue,
			OptionDesc("Collection type (supported types are: hash, btree)", "TYPE")));

  getopt = new GetOpt(getExtName(), opts);
}

int COLUpdateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME COLLECTION [HINTS]\n";
  return 1;
}

int COLUpdateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  getopt->displayOpt("COLLECTION", "Collection (can be a collection name, a collection oid or an OQL query)");
  getopt->displayOpt("HINTS", "Collection hints");
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
  const char *collection = argv[1].c_str();

  const char *hints = "";
  if (argv.size() > 2)
    hints = argv[2].c_str();

  IndexImpl::Type type;
  if (map.find(TYPE_OPT) != map.end()) {
    const char *typeOption = map[TYPE_OPT].value.c_str();

    if (!strcmp(typeOption, "hash"))
      type = IndexImpl::Hash;
    else if (!strcmp(typeOption, "btree"))
      type = IndexImpl::BTree;
    else
      return help();
  }

  conn.open();

  db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  LinkedList list;
  if (get_collections(collection, list))
    return 1;

  IndexImpl *idximpl;
  Status s = IndexImpl::make(db, type, hints, idximpl);

  LinkedListCursor c(list);
  Collection *coll;
  while (c.getNext((void *&)coll)) {
    coll->setImplementation(new CollImpl(CollAttrImpl::Unknown, idximpl)); // 0: must be correct implementation 2009-03-31
    s = coll->store();
  }
    
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

  opts.push_back(Option(FULL_OPT, OptionBoolType(), 0,
		 OptionDesc("Displays complete information")));

  opts.push_back(Option(FORMAT_OPT, 
			OptionStringType(),
			Option::MandatoryValue,
			OptionDesc("Statistics format", "FORMAT")));

  opts.push_back(Option(TYPE_OPT, 
			OptionStringType(),
			Option::MandatoryValue,
			OptionDesc("Index type (supported types are: hash, btree)", "TYPE")));

  getopt = new GetOpt(getExtName(), opts);
}

int COLSimulateCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME COLLECTION [HINTS]\n";
  return 1;
}

int COLSimulateCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  getopt->displayOpt("COLLECTION", "Collection (can be a collection name, a collection oid or an OQL query)");
  getopt->displayOpt("HINTS", "Collection hints");
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
  const char *collection = argv[1].c_str();

  const char *hints = "";
  if (argv.size() > 2)
    hints = argv[2].c_str();

  IndexImpl::Type type;
  if (map.find(TYPE_OPT) != map.end()) {
    const char *typeOption = map[TYPE_OPT].value.c_str();

    if (!strcmp(typeOption, "hash"))
      type = IndexImpl::Hash;
    else if (!strcmp(typeOption, "btree"))
      type = IndexImpl::BTree;
    else
      return help();
  }

  bool full = map.find(FULL_OPT) != map.end();

  const char *fmt = 0;
  if (map.find(FORMAT_OPT) != map.end())
    fmt = map[FORMAT_OPT].value.c_str();

  conn.open();

  db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  IndexImpl *idximpl;

  Status s = IndexImpl::make(db, type, hints, idximpl);
    
  LinkedList list;
  if (get_collections( collection, list))
    return 1;

  LinkedListCursor c(list);
  Collection *coll;
  CollImpl *collimpl = new CollImpl((CollAttrImpl::Type)idximpl->getType(), idximpl);

  int nn = 0;
  for (; c.getNext((void *&)coll); nn++) {
    if (fmt && idximpl->getType() == IndexImpl::Hash) {
      IndexStats *stats1, *stats2 = 0;
      if (coll->asCollArray())
	s = coll->asCollArray()->simulate(*collimpl, stats1, stats2);
      else
	s = coll->simulate(*collimpl, stats1);

      if (nn)
	fprintf(stdout, "\n");

      s = stats1->asHashIndexStats()->printEntries(fmt);

      delete stats1;
      if (stats2) {
	s = stats2->asHashIndexStats()->printEntries(fmt);
	
	delete stats2;
      }
    }
    else {
      std::string stats1, stats2;
      if (coll->asCollArray())
	s = coll->asCollArray()->simulate(*collimpl, stats1, stats2, True, (full)? eyedb::True : eyedb::False, "  ");
      else
	s = coll->simulate(*collimpl, stats1, True, (full)? eyedb::True : eyedb::False, "  ");

      if (nn)
	fprintf(stdout, "\n");

      collection_trace(coll);

      fprintf(stdout, stats1.c_str());
      if (stats2.size())
	fprintf(stdout, stats2.c_str());
    }
  }

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
  std::cerr << " DBNAME COLLECTION...\n";
  return 1;
}

int COLListCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  getopt->displayOpt("COLLECTION", "Collection (can be a collection name, a collection oid or an OQL query)");
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

  for (int n = 1; n < argv.size(); n++) {
    LinkedList list;
    if (get_collections(argv[n].c_str(), list))
      return 1;

    LinkedListCursor c(list);
    Collection *coll;
    for (int n = 0; c.getNext((void *&)coll); n++) {
      CollImpl *collimpl;
      Status s = coll->getImplementation(collimpl, True);

      if (n)
	fprintf(stdout, "\n");

      collection_trace(coll);
      if (collimpl->getIndexImpl()) {
	fprintf(stdout, collimpl->getIndexImpl()->toString("  ").c_str());
      }
    }
  }

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

  opts.push_back(Option(FULL_OPT, OptionBoolType(), 0,
		 OptionDesc("Displays complete information")));

  opts.push_back(Option(FORMAT_OPT, 
			OptionStringType(),
			Option::MandatoryValue,
			OptionDesc("Statistics format", "FORMAT")));

  getopt = new GetOpt(getExtName(), opts);
}

int COLStatsCmd::usage()
{
  getopt->usage("", "");
  std::cerr << " DBNAME COLLECTION...\n";
  return 1;
}

int COLStatsCmd::help()
{
  stdhelp();
  getopt->displayOpt("DBNAME", "Database name");
  getopt->displayOpt("COLLECTION", "Collection (can be a collection name, a collection oid or an OQL query)");
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

  bool full = map.find(FULL_OPT) != map.end();

  const char *fmt = 0;
  if (map.find(FORMAT_OPT) != map.end())
    fmt = map[FORMAT_OPT].value.c_str();

  conn.open();

  db = new Database(dbName);

  db->open(&conn, Database::DBRW);
  
  db->transactionBeginExclusive();

  int nn = 0;
  for (int n = 1; n < argv.size(); n++) {
    Status s;
    LinkedList list;
    if (get_collections(argv[n].c_str(), list))
      return 1;
    
    LinkedListCursor c(list);
    Collection *coll;
    for (; c.getNext((void *&)coll); nn++) {
      CollImpl *_collimpl = 0;
      s = coll->getImplementation(_collimpl);

      if (fmt && _collimpl->getIndexImpl()->getType() == IndexImpl::Hash) {
	IndexStats *stats1, *stats2 = 0;
	if (coll->asCollArray())
	  s = coll->asCollArray()->getImplStats(stats1, stats2);
	else
	  s = coll->getImplStats(stats1);

	if (nn) fprintf(stdout, "\n");
	s = stats1->asHashIndexStats()->printEntries(fmt);

	delete stats1;
	if (stats2) {
	  s = stats2->asHashIndexStats()->printEntries(fmt);

	  delete stats2;
	}
      }
      else {
	std::string stats1, stats2;
	if (coll->asCollArray())
	  s = coll->asCollArray()->getImplStats(stats1, stats2,	True, (full)? eyedb::True : eyedb::False, "  ");
	else
	  s = coll->getImplStats(stats1, True, (full)? eyedb::True : eyedb::False, "  ");

	if (nn) fprintf(stdout, "\n");
	collection_trace(coll);
	fprintf(stdout, stats1.c_str());
	if (stats2.size())
	  fprintf(stdout, stats2.c_str());
      }
      if (_collimpl)
	_collimpl->release();
    }
  }

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
