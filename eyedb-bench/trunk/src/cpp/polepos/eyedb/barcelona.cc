#include <vector>
#include <stdio.h>
#include "barcelona.h"
#include "polepos.h"

using namespace std;

void Barcelona::prepare()
{
  loadProperties( "eyedb.properties");

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  string dbName;
  getStringProperty( "database", dbName);

  conn = new eyedb::Connection( true);
  database = new poleposDatabase( conn, dbName.c_str(), eyedb::Database::DBRW);
}

void Barcelona::finish()
{
  database->close();
  //  conn->
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

  getIntProperty( "objects", objects);
  getIntProperty( "selects", selects);

  for (int i = 0; i < objects.size(); i++) {

    cout << "Running bench for " << objects[i] << " objects, " << selects[i] << " selects" << endl;

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

    report();

    getStopwatch().reset();
  }
}

int main(int argc, char *argv[])
{
  polepos initializer(argc, argv);

  Barcelona b;

  b.bench();
}
