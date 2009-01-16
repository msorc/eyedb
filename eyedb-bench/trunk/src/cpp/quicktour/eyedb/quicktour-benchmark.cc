#include <vector>
#include "quicktour.h"
#include "quicktour-benchmark.h"

using namespace std;

const char* QuicktourBenchmark::getImplementation() const
{
  string info = "EyeDB C++ implementation";

  string mode;
  if (getProperties().getStringProperty( "mode", mode) && mode == "local")
    info += " local mode";

  return info.c_str();
}

void QuicktourBenchmark::prepare()
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  string dbName;
  getProperties().getStringProperty( "eyedb.database", dbName);

  try {
    conn = new eyedb::Connection( true);

    string mode;
    eyedb::Database::OpenFlag flags;
    if (getProperties().getStringProperty( "eyedb.mode", mode) && mode == "local")
      flags = eyedb::Database::DBRWLocal;
    else
      flags = eyedb::Database::DBRW;

    database = new quicktourDatabase( conn, dbName.c_str(), flags);
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void QuicktourBenchmark::finish()
{
  try {
    database->close();
    conn->close();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

Teacher** QuicktourBenchmark::fillTeachers( int nTeachers) throw( eyedb::Exception)
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

Course** QuicktourBenchmark::fillCourses( int nCourses, Teacher** teachers, int nTeachers) throw( eyedb::Exception)
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
    courses[n]->setTeacher( teachers[ random() % nTeachers]);
  }

  getDatabase()->transactionCommit();

  return courses;
}


void QuicktourBenchmark::create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
{
  try {
    Teacher** teachers = fillTeachers( nTeachers);
    Course** courses = fillCourses( nCourses, teachers, nTeachers);

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
	student->addToCoursesColl( courses[ (i+c)%nCourses]);
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

void QuicktourBenchmark::queryByClass( const char *className, int nSelects)
{
  try {
    getDatabase()->transactionBegin();

    for ( int n = 0;  n < nSelects; n++) {
      char tmp[256];
      sprintf( tmp, "select x from %s as x where x.lastName ~ \"%d\"", className, n);

      eyedb::OQL q(getDatabase(), tmp);
      eyedb::ObjectArray arr;

      q.execute(arr);

      for (int i = 0; i < arr.getCount(); i++) {
	eyedb::Object *o = arr[i];
      }
    }
    
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void QuicktourBenchmark::query( int nSelects)
{
  queryByClass( "Teacher", nSelects);
  queryByClass( "Student", nSelects);
}

void QuicktourBenchmark::removeByClass( const char *className)
{
  try {
    getDatabase()->transactionBegin();
            
    char tmp[256];
    sprintf( tmp, "select x from %s as x", className);

    eyedb::OQL q(getDatabase(), tmp);
    eyedb::ObjectArray arr;
    
    q.execute(arr);

    for (int i = 0; i < arr.getCount(); i++) {
      eyedb::Object *o = arr[i];
      o->remove();
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void QuicktourBenchmark::remove()
{
  removeByClass( "Teacher");
  removeByClass( "Course");
  removeByClass( "Student");
}

void QuicktourBenchmark::run()
{
  vector<int> students;
  vector<int> courses;
  vector<int> teachers;
  vector<int> objectsPerTransaction;
  vector<int> selects;

  getProperties().getIntProperty( "quicktour.students", students);
  getProperties().getIntProperty( "quicktour.courses", courses);
  getProperties().getIntProperty( "quicktour.teachers", teachers);
  getProperties().getIntProperty( "quicktour.objects_per_transaction", objectsPerTransaction);
  getProperties().getIntProperty( "quicktour.selects", selects);
  
  getResult().addHeader( "students");
  getResult().addHeader( "courses");
  getResult().addHeader( "teachers");
  getResult().addHeader( "objectsPerTransaction");
  getResult().addHeader( "selects");

  for (int i = 0; i < students.size(); i++) {

    getResult().addValue( students[i]);
    getResult().addValue( courses[i]);
    getResult().addValue( teachers[i]);
    getResult().addValue( objectsPerTransaction[i]);
    getResult().addValue( selects[i]);

    getStopwatch().start();

    create( students[i], courses[i], teachers[i], objectsPerTransaction[i]);
    getStopwatch().lap( "create");

    query( selects[i]);
    getStopwatch().lap( "query");

    remove();
    getStopwatch().lap( "remove");

    getStopwatch().stop();

    getResult().addLaps( getStopwatch().getLaps());

    getStopwatch().reset();

    getResult().next();
  }
}

int main(int argc, char *argv[])
{
  QuicktourBenchmark b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  quicktour initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  int c;
  b.getProperties().getIntProperty("reporter.simple.column_width", c);
  r.setColumnWidth( c);
  r.report(b);
}
