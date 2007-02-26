
#include "dspsch.h"

int
main(int argc, char *argv[])
{
  EyeDB::init(argc, argv);


  const char *dbname = argc > 1 ? argv[1] : "EYEDBDBM";

  idbException::setMode(idbException::ExceptionMode);

  try {
    idbConnection conn;
    conn.open();
    Database db(dbname);
    db.open(&conn, Database::DBRW);

    db->transactionBegin();
    db->transactionCommit();
  }

  catch(idbException &e) {
    cerr << e << endl;
    return 1;
  }
}

