#ifndef _QUICKTOUR_BENCHMARK_H_
#define _QUICKTOUR_BENCHMARK_H_

#include <eyedb/eyedb.h>
#include "framework/benchmark.h"

class QuicktourBenchmark : public eyedb::benchmark::Benchmark {
 public:
  void prepare();
  void finish();

  const char* getRunDescription() const;

  eyedb::Database *getDatabase() { return database; }

private:
  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
