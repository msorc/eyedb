#include "odl_quicktour_array.h"
#include "quicktour_benchmark.h"

using namespace std;

const char* QuicktourBenchmarkArray::getImplementation() const
{
  string info = "EyeDB C++ implementation using arrays";

  string mode;
  if (getProperties().getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

eyedb::Database *QuicktourBenchmarkArray::openDatabase( eyedb::Connection *conn, const char *dbName, eyedb::Database::OpenFlag flags) throw( eyedb::Exception)
{
  return new odl_quicktour_arrayDatabase( conn, dbName, flags);
}

Teacher** QuicktourBenchmarkArray::fillTeachers( int nTeachers) throw( eyedb::Exception)
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

Course** QuicktourBenchmarkArray::fillCourses( int nCourses) throw( eyedb::Exception)
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

void QuicktourBenchmarkArray::relationTeacherCourse( Teacher** teachers, int nTeachers, Course** courses, int nCourses) throw( eyedb::Exception)
{
  for ( int n = 0; n < nCourses; n++) {
    Teacher *teacher = teachers[ random() % nTeachers];
    courses[n]->setTeacher( teacher);

    unsigned int last = teacher->getCoursesCount();
    teacher->setCoursesCount( last+1);
    teacher->setCourses( last, courses[n]);
  }
}

void QuicktourBenchmarkArray::create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
{
  try {
    Teacher** teachers = fillTeachers( nTeachers);
    Course** courses = fillCourses( nCourses);
    relationTeacherCourse( teachers, nTeachers, courses, nCourses);

    getDatabase()->transactionBegin();
    
    for (int c = 0; c < nCourses; c++)
      courses[c]->setStudentsCount( nStudents);
            
    for ( int n = 0; n < nStudents; n++) {
      Student *student = new Student( getDatabase());

      char tmp[256];
      sprintf( tmp, "Student_%d_firstName", n);
      student->setFirstName( tmp);
      sprintf( tmp, "Student_%d", n);
      student->setLastName( tmp);
      student->setBeginYear( (short)((random()%3) + 1));

      student->setCoursesCount( nCourses);

#if 0
      for ( int c = 0; c < nCourses; c++) {
	cout << "setting course " << c << " for student " << n << endl;

	student->setCourses( c, courses[c]);
	courses[c]->setStudents( n, student);
      }
#endif

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

  QuicktourBenchmarkArray b;
  b.setProperties( properties);

  odl_quicktour_array initializer(argc, argv);

  b.bench();

  eyedb::benchmark::DefaultReporter r;
  r.report(b);
}
