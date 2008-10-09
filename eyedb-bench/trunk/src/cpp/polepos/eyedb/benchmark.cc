#include <eyedb/eyedb.h>
#include "benchmark.h"

void Benchmark::prepare()
{
  eyedb::init();
  polepos::init();

  eyedb::Exception::setMode( eyedb::Exception::ExceptionMode);

  try {
    _conn = new Connection();

    _conn->open();

    _database = new PoleposDatabase( "polepos");

    _database->open( &conn, eyedb::Database::DBRW);
  }
  catch(eyedb::Exception &e) {
    e.print();
  }
}

void Benchmark::finish()
{
  polepos::release();
  eyedb::release();
}
