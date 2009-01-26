#include <string>
#include "array_odl.h"

using namespace std;

class TestArray {
 public:
  TestArray( string dbName, int nA, int nB) : dbName( dbName), nA(nA), nB(nB) {}

  string getDbName() { return dbName; }
  int getNA() { return nA; }
  int getNB() { return nB; }

  void prepare() throw( eyedb::Exception);
  void finish() throw( eyedb::Exception);

  void create() throw( eyedb::Exception);

private:
  eyedb::Database *getDatabase() { return database; }

  void fillA() throw( eyedb::Exception);
  void fillB() throw( eyedb::Exception);
  void associate() throw( eyedb::Exception);

  string dbName;
  int nA;
  int nB;
  A **aa;
  B **bb;

  eyedb::Connection *conn;
  eyedb::Database *database;
};

void TestArray::prepare() throw( eyedb::Exception)
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  conn = new eyedb::Connection( true);

   eyedb::Database::OpenFlag flags;
   flags = eyedb::Database::DBRWLocal;

   database = new array_odlDatabase( conn, getDbName().c_str(), flags);
}

void TestArray::finish() throw( eyedb::Exception)
{
  database->close();
  conn->close();
}

void TestArray::fillA() throw( eyedb::Exception)
{
  aa = new A*[getNA()];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < getNA(); n++) {
    aa[n] = new A( getDatabase());

    char tmp[256];
    sprintf( tmp, "A_%d", n);
    aa[n]->setName( tmp);
  }

  getDatabase()->transactionCommit();
}

void TestArray::fillB() throw( eyedb::Exception)
{
  bb = new B*[getNB()];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < getNB(); n++) {
    bb[n] = new B( getDatabase());

    char tmp[256];
    sprintf( tmp, "B_%d", n);
    bb[n]->setName( tmp);
  }

  getDatabase()->transactionCommit();
}

void TestArray::associate() throw( eyedb::Exception)
{
  int ia, ib;

  getDatabase()->transactionBegin();

  // setting the array size for each object
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

#define FULLRECURS

#ifdef FULLRECURS
    cout << "storing A " << ia << " full recurs..." << endl;
    aa[ia]->store( eyedb::FullRecurs);
#else
    cout << "storing A " << ia << " no recurs..." << endl;
    aa[ia]->store(eyedb::NoRecurs);
    cout << "-> " << aa[ia]->getOid() << endl;
    for ( int ib = 0; ib < getNB(); ib++) {
      bb[ib]->store(eyedb::NoRecurs);
    }
    aa[ia]->store(eyedb::NoRecurs);
#endif
  }

  getDatabase()->transactionCommit();
}

void TestArray::create() throw( eyedb::Exception)
{
  fillA();
  fillB();
  associate();
}

int main( int argc, char **argv)
{
  array_odl initializer(argc, argv);

  int nA = 5, nB = 5;

  if (argc >= 3)
    sscanf( argv[2], "%d", &nA);
  if (argc >= 4)
    sscanf( argv[3], "%d", &nB);

  try {
    TestArray t( argv[1], nA, nB);
    t.prepare();
    t.create();
    t.finish();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}
