#ifndef _BARCELONA_H_
#define _BARCELONA_H_

#include <eyedb/eyedb.h>
#include "benchmark.h"

class Barcelona : public eyedb::benchmark::Benchmark {

public:
  Barcelona()
  {
  }

  void write( int count);
  void read();
  void query( int selectCount);
  void destroy();

  void prepare();
  void run();
  void finish();

  const char* getName() { return "Barcelona"; }
  const char* getDescription() { return "writes, reads, queries and deletes objects with a 5 level inheritance structure"; }

  eyedb::Database *getDatabase() { return database; }

private:
  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
