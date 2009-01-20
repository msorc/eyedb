#include "odl_quicktour_collection.h"
#include "quicktour_benchmark.h"

using namespace std;

const char* QuicktourBenchmarkCollection::getImplementation() const
{
  string info = "EyeDB C++ implementation using collections";

  string mode;
  if (getProperties().getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

eyedb::Database *QuicktourBenchmarkCollection::openDatabase( eyedb::Connection *conn, const char *dbName, eyedb::Database::OpenFlag flags) throw( eyedb::Exception)
{
  return new odl_quicktour_collectionDatabase( conn, dbName, flags);
}

Teacher** QuicktourBenchmarkCollection::fillTeachers( int nTeachers) throw( eyedb::Exception)
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

Course** QuicktourBenchmarkCollection::fillCourses( int nCourses) throw( eyedb::Exception)
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

void QuicktourBenchmarkCollection::relationTeacherCourse( Teacher** teachers, int nTeachers, Course** courses, int nCourses) throw( eyedb::Exception)
{
  for ( int n = 0; n < nCourses; n++) {
    Teacher *teacher = teachers[ random() % nTeachers];
    courses[n]->setTeacher( teacher);
    teacher->addToCoursesColl( courses[n]);
  }
}

void QuicktourBenchmarkCollection::create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
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

  QuicktourBenchmarkCollection b;
  b.setProperties( properties);

  odl_quicktour_collection initializer(argc, argv);

  b.bench();

  eyedb::benchmark::DefaultReporter r;
  r.report(b);
}
