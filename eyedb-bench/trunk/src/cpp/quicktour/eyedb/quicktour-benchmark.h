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
  const char* getDescription() const { return "Quicktour benchmark, create, query and delete objects with one-level inheritance, one-to-many and many-to-many relations with referential integrity check"; }
  const char* getImplementation() const;

  eyedb::Database *getDatabase() { return database; }

private:
  Teacher** fillTeachers( int nTeachers) throw( eyedb::Exception);
  Course** fillCourses( int nCourses, Teacher** teachers, int nTeachers) throw( eyedb::Exception);

  void queryByClass( const char *className, int nSelects);

  void removeByClass( const char *className);

  eyedb::Database *database;
  eyedb::Connection *conn;
};

#endif
