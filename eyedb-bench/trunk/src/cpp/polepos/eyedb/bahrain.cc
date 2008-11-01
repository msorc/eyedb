#include <sstream>
#include <vector>
#include "polepos.h"
#include "bahrain.h"

using namespace std;

void Bahrain::write( int count)
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

void Bahrain::read()
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

void Bahrain::query( int selectCount)
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

void Bahrain::destroy()
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

void Bahrain::run()
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
  Bahrain b;
  b.loadProperties( "eyedb.properties");
  b.loadProperties( argc, argv);

  polepos initializer(argc, argv);

  b.bench();
}
