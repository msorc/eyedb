#ifndef _BARCELONA_H_
#define _BARCELONA_H_

#include <eyedb/eyedb.h>
#include "benchmark.h"

class Barcelona : public eyedb::benchmark::Benchmark {

public:
  Barcelona()
  {
  }

  void write();
  void read();
  void query();
  void destroy();

  void prepare();
  void run();
  void finish();

  const char* getName() { return "Barcelona"; }
  const char* getDescription() { return "Creating, reading, querying, destroying objects with 5 levels of inheritance"; }

  eyedb::Database *getDatabase() { return database; }

private:
  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
