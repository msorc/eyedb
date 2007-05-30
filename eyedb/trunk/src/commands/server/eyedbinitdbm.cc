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
  Author: Francois Dechelle <francois@dechelle.net>
*/

#include "eyedbconfig.h"
#include <eyedb/eyedb.h>
#include "eyedb/DBM_Database.h"
//#include <eyedb/opts.h>

using namespace eyedb;

const std::string PROG_NAME = "eyedbinitdbm";

static const char DBS_EXT[] = ".dbs";
static const char DAT_EXT[] = ".dat";

static const int ONE_K = 1024;
static const int DEFAULT_DATSIZE = 2048;
static const int DEFAULT_DATSZSLOT = 16;
static const char DEFAULT_DATNAME[] = "DEFAULT";
static const unsigned int DEFAULT_DATOBJCNT = 10000000;

//    echo ==== Creating EYEDBDBM database
//    $bindir/eyedbadmin dbmcreate --strict-unix=@
//    echo ==== Setting EYEDBDBM database permissions
//    $bindir/eyedbadmin dbaccess EYEDBDBM r --user=@

#if 0
// from old eyedbadmin
static int
dbmcreate_realize(char *username, char *passwd, char *dbfile, char *datafiles[], int datafiles_cnt)
{
  if (dbfile)
    return usage(prog);

  DbCreateDescription dbdesc;

  DBM_Database *dbm = new DBM_Database(dbmdb);

  if (dbcreate_prologue(dbm, DBM_Database::getDbName(), dbfile,
			datafiles, datafiles_cnt, (const char *)0, &dbdesc))
    return 1;

  Status status = dbm->create(conn, passwdauth, username, passwd, &dbdesc);

  CHECK(status);
  return 0;
}
#endif

static int
help()
{
  std::cerr << "Create the EyeDB EYEDBDBM system database.";
  std::cerr << "This command must be run once after installing EyeDB";
  return 1;
}

static int
initdbm_perform( eyedb::Connection &conn, std::vector<std::string> &argv)
{
#if 0
  bool r = getopt->parse(PROG_NAME, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find("help") != map.end()) {
    return help();
  }
#endif

  conn.open();

  DBM_Database *dbm = new DBM_Database( Database::getDefaultDBMDB());

  DbCreateDescription dbdesc;

  strcpy( dbdesc.dbfile, Database::getDefaultServerDBMDB());
  dbdesc.sedbdesc.dbid = 0;
  dbdesc.sedbdesc.nbobjs = DEFAULT_DATOBJCNT;
  dbdesc.sedbdesc.ndat = 1;

  // FIXME
  std::string datfile = std::string("dbmdb") + DAT_EXT;
  strcpy( dbdesc.sedbdesc.dat[0].file, datfile.c_str());
  strcpy( dbdesc.sedbdesc.dat[0].name, DEFAULT_DATNAME);
  dbdesc.sedbdesc.dat[0].maxsize = DEFAULT_DATSIZE * ONE_K;
  dbdesc.sedbdesc.dat[0].mtype = eyedbsm::BitmapType;
  dbdesc.sedbdesc.dat[0].sizeslot = DEFAULT_DATSZSLOT;
  dbdesc.sedbdesc.dat[0].dtype = eyedbsm::LogicalOidType;
  dbdesc.sedbdesc.dat[0].dspid = 0;

  // FIXME
  //  dbm->create(&conn, passwdauth, username, passwd, &dbdesc);

  return 0;
}

int
main(int c_argc, char *c_argv[])
{
  eyedb::init(c_argc, c_argv);

  std::vector<std::string> argv;
  for (int n = 1; n < c_argc; n++)
    argv.push_back(c_argv[n]);

  Exception::setMode(Exception::ExceptionMode);

  try {
    Connection conn;

    return initdbm_perform(conn, argv);
  }
  catch(Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
