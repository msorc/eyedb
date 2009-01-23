#include "odl_quicktour_noinverse_set.h"
#include "quicktour_benchmark.h"

using namespace std;

class QuicktourBenchmarkNoInverseSet : public QuicktourBenchmark {
public:
  const char* getImplementation() const;

  void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction);
protected:
  eyedb::Database *openDatabase( eyedb::Connection *conn, const char *dbName, eyedb::Database::OpenFlag flags) throw( eyedb::Exception);

  Teacher** fillTeachers( int nTeachers) throw( eyedb::Exception);
  Course** fillCourses( int nCourses) throw( eyedb::Exception);
  void relationTeacherCourse( Teacher** teachers, int nTeachers, Course** courses, int nCourses) throw( eyedb::Exception);
};

const char* QuicktourBenchmarkNoInverseSet::getImplementation() const
{
  string info = "EyeDB C++ implementation using sets and no referential integrity";

  string mode;
  if (getProperties().getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

eyedb::Database *QuicktourBenchmarkNoInverseSet::openDatabase( eyedb::Connection *conn, const char *dbName, eyedb::Database::OpenFlag flags) throw( eyedb::Exception)
{
  return new odl_quicktour_noinverse_setDatabase( conn, dbName, flags);
}

Teacher** QuicktourBenchmarkNoInverseSet::fillTeachers( int nTeachers) throw( eyedb::Exception)
{
  Teacher **teachers = new Teacher*[nTeachers];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < nTeachers; n++) {
    teachers[n] = new Teacher( getDatabase());

    char tmp[256];
    sprintf( tmp, "Teacher_%d_firstName", n);
    teachers[n]->setFirstName( tmp);
    sprintf( tmp, "Teacher_%d", n);
    teachers[n]->setLastName( tmp);
  }

  getDatabase()->transactionCommit();
  
  return teachers;
}

Course** QuicktourBenchmarkNoInverseSet::fillCourses( int nCourses) throw( eyedb::Exception)
{
  Course **courses = new Course*[ nCourses];

  getDatabase()->transactionBegin();

  for ( int n = 0; n < nCourses; n++) {
    courses[n] = new Course( getDatabase());

    char tmp[256];
    sprintf( tmp, "Course_%d", n);
    courses[n]->setTitle( tmp);
    sprintf( tmp, "Description of course %d", n);
    courses[n]->setDescription( tmp);
  }

  getDatabase()->transactionCommit();

  return courses;
}

void QuicktourBenchmarkNoInverseSet::relationTeacherCourse( Teacher** teachers, int nTeachers, Course** courses, int nCourses) throw( eyedb::Exception)
{
  for ( int n = 0; n < nCourses; n++) {
    Teacher *teacher = teachers[ random() % nTeachers];
    courses[n]->setTeacher( teacher);
    teacher->addToCoursesColl( courses[n]);
  }
}

void QuicktourBenchmarkNoInverseSet::create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
{
  try {
    Teacher** teachers = fillTeachers( nTeachers);
    Course** courses = fillCourses( nCourses);
    relationTeacherCourse( teachers, nTeachers, courses, nCourses);

    getDatabase()->transactionBegin();
            
    for ( int n = 0; n < nStudents; n++) {
      Student *student = new Student( getDatabase());

      char tmp[256];
      sprintf( tmp, "Student_%d_firstName", n);
      student->setFirstName( tmp);
      sprintf( tmp, "Student_%d", n);
      student->setLastName( tmp);
      student->setBeginYear( (short)((random()%3) + 1));

      int i = random();
      for ( int c = 0; c < nCourses; c++) {
	Course *course = courses[ (i+c)%nCourses];
	student->addToCoursesColl( course);
	course->addToStudentsColl( student);
      }

      student->store( eyedb::FullRecurs);
      
      if (n % nObjectsPerTransaction == nObjectsPerTransaction - 1) {
	getDatabase()->transactionCommit();
	getDatabase()->transactionBegin();
      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

int main(int argc, char *argv[])
{
  eyedb::benchmark::Properties properties;
  properties.load( argv[1]);
  properties.load( argc, argv);

  QuicktourBenchmarkNoInverseSet b;
  b.setProperties( properties);

  odl_quicktour_noinverse_set initializer(argc, argv);

  b.bench();

  eyedb::benchmark::DefaultReporter r;
  r.report(b);
}
