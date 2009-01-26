#ifndef _QUICKTOUR_BENCHMARK_H_
#define _QUICKTOUR_BENCHMARK_H_

#include <eyedb/eyedb.h>
#include "framework/benchmark.h"

class Teacher;
class Course;
class Student;

class QuicktourBenchmark : public eyedb::benchmark::Benchmark {
 public:
  const char* getImplementation() const;

  void prepare();
  void finish();

  virtual void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction) = 0;
  void query( int nSelects);
  void remove();

  void run();

  const char* getName() const { return "Quicktour"; }
  const char* getDescription() const { return "Quicktour benchmark, create, query and delete objects with one-level inheritance, one-to-many and many-to-many relations"; }

  eyedb::Database *getDatabase() { return database; }

protected:
  virtual eyedb::Database *openDatabase( eyedb::Connection *conn, const char *dbName, eyedb::Database::OpenFlag flags) throw( eyedb::Exception) = 0;

  virtual Teacher** fillTeachers( int nTeachers) throw( eyedb::Exception) = 0;
  virtual Course** fillCourses( int nCourses) throw( eyedb::Exception) = 0;
  virtual void relationTeacherCourse( Teacher** teachers, int nTeachers, Course** courses, int nCourses) throw( eyedb::Exception) = 0;

private:
  void queryByClass( const char *className, int nSelects);
  void removeByClass( const char *className);

  eyedb::Database *database;
  eyedb::Connection *conn;

#ifdef LOOKING_FOR_BUG
  bool verbose;
#endif
};

#endif
