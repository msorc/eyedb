#include "polepos.h"
#include "polepos-benchmark.h"

using namespace std;

const char* PoleposBenchmark::getRunDescription()
{
  string info = "EyeDB C++ implementation";

  string mode;
  if (getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

void PoleposBenchmark::prepare()
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  string dbName;
  getStringProperty( "database", dbName);

  conn = new eyedb::Connection( true);

  string mode;
  eyedb::Database::OpenFlag flags;
  if (getStringProperty( "mode", mode) && mode == "local")
    flags = eyedb::Database::DBRWLocal;
  else
    flags = eyedb::Database::DBRW;

  database = new poleposDatabase( conn, dbName.c_str(), flags);
}

void PoleposBenchmark::finish()
{
  database->close();
  conn->close();
}

