#include <vector>
#include "quicktour_benchmark.h"

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

  int v;
  if (getProperties().getIntProperty( "verbose", v))
    cerr << "Opening database: " << dbName << endl;

  try {
    conn = new eyedb::Connection( true);

    string mode;
    eyedb::Database::OpenFlag flags;
    if (getProperties().getStringProperty( "eyedb.mode", mode) && mode == "local")
      flags = eyedb::Database::DBRWLocal;
    else
      flags = eyedb::Database::DBRW;

    database = openDatabase( conn, dbName.c_str(), flags);
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

void QuicktourBenchmark::queryByClass( const char *className, int nSelects)
{
  try {
    getDatabase()->transactionBegin();

    for ( int n = 0;  n < nSelects; n++) {
      char tmp[256];
      sprintf( tmp, "select x from %s as x where x.lastName = \"%s_%d\"", className, className, n);

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

#define LOOKING_FOR_BUG

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
#ifdef LOOKING_FOR_BUG
      cout << "removed object " << i << " of class " << className << endl;
#endif
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
  
  for (int i = 0; i < students.size(); i++) {

    create( students[i], courses[i], teachers[i], objectsPerTransaction[i]);

    query( selects[i]);

    remove();
  }
}

