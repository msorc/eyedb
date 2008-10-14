#include <vector>
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

void Barcelona::read( int count)
{
}

void Barcelona::query( int count)
{
}

void Barcelona::destroy( int count)
{
}

void Barcelona::run()
{
  vector<int> objects;

  getIntProperty( "objects", objects);

  for (int i = 0; i < objects.size(); i++) {
    getStopwatch().start();

    write( objects[i]);
    getStopwatch().lap( "write");

    read( objects[i]);
    getStopwatch().lap( "read");

    query( objects[i]);
    getStopwatch().lap( "query");

    destroy( objects[i]);
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

  b.prepare();
  b.bench();
  b.finish();
}
