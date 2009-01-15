#ifndef _QUICKTOUR_BENCHMARK_H_
#define _QUICKTOUR_BENCHMARK_H_

#include <eyedb/eyedb.h>
#include "framework/benchmark.h"

class QuicktourBenchmark : public eyedb::benchmark::Benchmark {
 public:
  void prepare();
  void finish();

  void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction);
  void query( int nSelects);
  void remove();

  void run();

  const char* getName() const { return "Quicktour"; }
  const char* getDescription() const { return "writes, reads, queries and deletes objects with a 5 level inheritance structure"; }
  const char* getImplementation() const;

  eyedb::Database *getDatabase() { return database; }

private:
  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
