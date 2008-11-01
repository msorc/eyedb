#ifndef _POLEPOS_BENCHMARK_H_
#define _POLEPOS_BENCHMARK_H_

#include <eyedb/eyedb.h>
#include "benchmark/benchmark.h"

class PoleposBenchmark : public eyedb::benchmark::Benchmark {
 public:
  void prepare();
  void finish();

  const char* getRunDescription();

  eyedb::Database *getDatabase() { return database; }

private:
  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
