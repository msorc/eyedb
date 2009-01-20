#include <string>
#include "array.h"

using namespace std;

class Test {
 public:
  Test( string dbName, int nA, int nB) : dbName( dbName), nA(nA), nB(nB) {}

  string getDbName() { return dbName; }
  int getNA() { return nA; }
  int getNB() { return nB; }

  eyedb::Database *getDatabase() { return database; }

  void prepare() throw( eyedb::Exception);
  void finish() throw( eyedb::Exception);

  A** fillA() throw( eyedb::Exception);
  B** fillB() throw( eyedb::Exception);
  void create() throw( eyedb::Exception);

private:
  string dbName;
  int nA;
  int nB;

  eyedb::Connection *conn;
  eyedb::Database *database;
};

void Test::prepare() throw( eyedb::Exception)
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  conn = new eyedb::Connection( true);

   eyedb::Database::OpenFlag flags;
   flags = eyedb::Database::DBRW;

   database = new arrayDatabase( conn, getDbName().c_str(), flags);
}

void Test::finish() throw( eyedb::Exception)
{
  database->close();
  conn->close();
}

A** Test::fillA() throw( eyedb::Exception)
{
  A **aa = new A*[getNA()];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < getNA(); n++) {
    aa[n] = new A( getDatabase());

    char tmp[256];
    sprintf( tmp, "A_%d", n);
    aa[n]->setName( tmp);
  }

  getDatabase()->transactionCommit();
  
  return aa;
}

B** Test::fillB() throw( eyedb::Exception)
{
  B **bb = new B*[getNB()];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < getNB(); n++) {
    bb[n] = new B( getDatabase());

    char tmp[256];
    sprintf( tmp, "B_%d", n);
    bb[n]->setName( tmp);
  }

  getDatabase()->transactionCommit();
  
  return bb;
}

void Test::create() throw( eyedb::Exception)
{
  A **aa = fillA();
  B **bb = fillB();
  int ia, ib;

  getDatabase()->transactionBegin();

  for ( ia = 0; ia < getNA(); ia++)
    aa[ia]->setBCount( getNB());
  for ( ib = 0; ib < getNB(); ib++)
    bb[ib]->setACount( getNA());

  for ( ia = 0; ia < getNA(); ia++) {
    for ( int ib = 0; ib < getNB(); ib++) {
      cout << "setting A " << ia << " and B " << ib << endl;

      aa[ia]->setB( ib, bb[ib]);
      bb[ib]->setA( ia, aa[ia]);
    }

    aa[ia]->store( eyedb::FullRecurs);
  }

  getDatabase()->transactionCommit();
}

int main( int argc, char **argv)
{
  array initializer(argc, argv);

  int nA = 5, nB = 5;

  if (argc >= 3)
    sscanf( argv[2], "%d", &nA);
  if (argc >= 4)
    sscanf( argv[3], "%d", &nB);

  try {
    Test t( argv[1], nA, nB);
    t.prepare();
    t.create();
    t.finish();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}
