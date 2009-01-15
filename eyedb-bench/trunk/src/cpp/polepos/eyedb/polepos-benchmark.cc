#include "polepos.h"
#include "polepos-benchmark.h"

using namespace std;

const char* PoleposBenchmark::getImplementation() const
{
  string info = "EyeDB C++ implementation";

  string mode;
  if (getProperties().getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

void PoleposBenchmark::prepare()
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  string dbName;
  getProperties().getStringProperty( "database", dbName);

  try {
    conn = new eyedb::Connection( true);

    string mode;
    eyedb::Database::OpenFlag flags;
    if (getProperties().getStringProperty( "mode", mode) && mode == "local")
      flags = eyedb::Database::DBRWLocal;
    else
      flags = eyedb::Database::DBRW;

    database = new poleposDatabase( conn, dbName.c_str(), flags);
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void PoleposBenchmark::finish()
{
  try {
    database->close();
    conn->close();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

