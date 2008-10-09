#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

#include <eyedb/eyedb.h>

class Benchmark : public eyedb::benchmark::Benchmark {
public:
  Benchmark();
  void prepare();
  void finish();

protected:
  eyedb::Database *getDatabase()
  {
    return _database;
  }

private:
  eyedb::Database *_database;
  eyedb::Connection *_conn;
};
#endif
