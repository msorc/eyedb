#include <string>
#include "array.h"

using namespace std;

class Test {
 public:
  Test( string dbName, int nA, int nB) : dbName( dbName), nA(nA), nB(nB) {}

  string getDbName() { return dbName; }
  int getNA() { return nA; }
  int getNB() { return nB; }

  void prepare();
  void finish();

  eyedb::Database *getDatabase() { return database; }

  A** fillA() throw( eyedb::Exception);
  B** fillB() throw( eyedb::Exception);

private:
  string dbName;
  int nA;
  int nB;

  eyedb::Database *database;
  eyedb::Connection *conn;
};

void Test::prepare()
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    conn = new eyedb::Connection( true);

    eyedb::Database::OpenFlag flags;
    flags = eyedb::Database::DBRW;

    database = openDatabase( conn, getDbName().c_str(), flags);
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Test::finish()
{
  try {
    database->close();
    conn->close();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
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



main()
{
  Test t( argv[1], 10, 20);

}
