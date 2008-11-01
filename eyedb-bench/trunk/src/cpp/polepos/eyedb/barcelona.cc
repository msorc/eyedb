#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "barcelona.h"
#include "polepos.h"

using namespace std;

const char* Barcelona::getRunDescription()
{
  string info = "EyeDB C++ implementation";

  string mode;
  if (getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

void Barcelona::prepare()
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  string dbName;
  getStringProperty( "database", dbName);

  conn = new eyedb::Connection( true);

  string mode;
  eyedb::Database::OpenFlag flags;
  if (getStringProperty( "mode", mode) && mode == "local")
    flags = eyedb::Database::DBRWLocal;
  else
    flags = eyedb::Database::DBRW;

  database = new poleposDatabase( conn, dbName.c_str(), flags);
}

void Barcelona::finish()
{
  database->close();
}

void Barcelona::write( int count)
{
  try {
    getDatabase()->transactionBegin();
            
    for ( int i = 0; i < count; i++) {
      B4 *b4 = new B4( getDatabase());

      b4->setB0( i+1);
      b4->setB1( i+1);
      b4->setB2( i+1);
      b4->setB3( i+1);
      b4->setB4( i+1);

      b4->store( eyedb::FullRecurs);
    }
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Barcelona::read()
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select b from B4 as b");
    eyedb::ObjectArray arr;

    q.execute(arr);

    int s = 0;
    for (int i = 0; i < arr.getCount(); i++) {
      B4 *b = B4_c(arr[i]);
      s += b->getB4();
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Barcelona::query( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, "select b from B4 as b where b.b2=%d", i+1);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

      int s = 0;
      for (int i = 0; i < arr.getCount(); i++) {
	B4 *b = B4_c(arr[i]);
	s += b->getB4();
      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Barcelona::destroy()
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select b from B4 as b");
    eyedb::ObjectArray arr;

    q.execute(arr);

    int s = 0;
    for (int i = 0; i < arr.getCount(); i++) {
      B4 *b = B4_c(arr[i]);
      b->remove();
      s += 5;
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Barcelona::run()
{
  vector<int> objects;
  vector<int> selects;

  addColumnHeader( "selects");
  addColumnHeader( "objects");
  addColumnHeader( "write (ms)");
  addColumnHeader( "read (ms)");
  addColumnHeader( "query (ms)");
  addColumnHeader( "destroy (ms)");
  addColumnHeader( "total (ms)");

  setColumnWidth( 15);

  getIntProperty( "objects", objects);
  getIntProperty( "selects", selects);
  
  for (int i = 0; i < objects.size(); i++) {

    ostringstream oss1;
    oss1 << selects[i];
    addRowHeader( oss1.str());

    ostringstream oss2;
    oss2 << objects[i];
    addRowHeader( oss2.str());

    getStopwatch().start();

    write( objects[i]);
    getStopwatch().lap( "write");

    read();
    getStopwatch().lap( "read");

    query( selects[i]);
    getStopwatch().lap( "query");

    destroy();
    getStopwatch().lap( "destroy");

    getStopwatch().stop();

    reportLaps();

    getStopwatch().reset();
  }
}

int main(int argc, char *argv[])
{
  Barcelona b;
  b.loadProperties( "eyedb.properties");
  b.loadProperties( argc, argv);

  polepos initializer(argc, argv);

  b.bench();
}
