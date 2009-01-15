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
  getProperties().getStringProperty( "database", dbName);

  try {
    conn = new eyedb::Connection( true);

    string mode;
    eyedb::Database::OpenFlag flags;
    if (getProperties().getStringProperty( "mode", mode) && mode == "local")
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

void QuicktourBenchmark::create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
{
  try {
    getDatabase()->transactionBegin();
            
    //o->store( eyedb::FullRecurs);
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void QuicktourBenchmark::query( int nSelects)
{
  try {
    getDatabase()->transactionBegin();
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void QuicktourBenchmark::remove()
{
  try {
    getDatabase()->transactionBegin();
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
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
  r.report(b);
}
