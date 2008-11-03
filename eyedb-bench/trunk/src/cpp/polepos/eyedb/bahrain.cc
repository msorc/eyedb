#include <sstream>
#include <vector>
#include "polepos.h"
#include "bahrain.h"

using namespace std;

void Bahrain::write( int count)
{
  try {
    getDatabase()->transactionBegin();
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::query_indexed_string( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, "select b from B4 as b where b.b2=%d", i+1);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

//       int s = 0;
//       for (int i = 0; i < arr.getCount(); i++) {
// 	B4 *b = B4_c(arr[i]);
// 	s += b->getB4();
//      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::query_string( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, "select b from B4 as b where b.b2=%d", i+1);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

//       int s = 0;
//       for (int i = 0; i < arr.getCount(); i++) {
// 	B4 *b = B4_c(arr[i]);
// 	s += b->getB4();
//      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::query_indexed_int( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, "select b from B4 as b where b.b2=%d", i+1);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

//       int s = 0;
//       for (int i = 0; i < arr.getCount(); i++) {
// 	B4 *b = B4_c(arr[i]);
// 	s += b->getB4();
//      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::query_int( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, "select b from B4 as b where b.b2=%d", i+1);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

//       int s = 0;
//       for (int i = 0; i < arr.getCount(); i++) {
// 	B4 *b = B4_c(arr[i]);
// 	s += b->getB4();
//      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::update( int updateCount)
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select b from B4 as b");
    eyedb::ObjectArray arr;

    q.execute(arr);

    int s = 0;
    for (int i = 0; i < arr.getCount(); i++) {
      B4 *b = B4_c(arr[i]);
      s += 5;
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
  vector<int> updates;

  getProperties().getIntProperty( "objects", objects);
  getProperties().getIntProperty( "selects", selects);
  getProperties().getIntProperty( "updates", selects);
  
  getResult().addHeader( "objects");
  getResult().addHeader( "selects");
  getResult().addHeader( "updates");

  for (int i = 0; i < objects.size(); i++) {

    getResult().addValue( objects[i]);
    getResult().addValue( selects[i]);
    getResult().addValue( updates[i]);

    getStopwatch().start();

    write( objects[i]);
    getStopwatch().lap( "write");

    query_indexed_string( selects[i]);
    getStopwatch().lap( "query_indexed_string");

    query_string( selects[i]);
    getStopwatch().lap( "query_string");

    query_indexed_int( selects[i]);
    getStopwatch().lap( "query_indexed_int");

    query_int( selects[i]);
    getStopwatch().lap( "query_int");

    update( updates[i]);
    getStopwatch().lap( "update");

    destroy();
    getStopwatch().lap( "destroy");

    getStopwatch().stop();

    getStopwatch().reset();
  }
}

int main(int argc, char *argv[])
{
  Bahrain b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  polepos initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  r.report(b);
}
