#include <sstream>
#include <vector>
#include <ctype.h>
#include "polepos.h"
#include "bahrain.h"

using namespace std;

void Bahrain::write( int count, int commitInterval)
{
  try {
    getDatabase()->transactionBegin();
            
    int commitCount = 0;

    for ( int i = 0; i < count; i++ ){
      IndexedPilot *p = new IndexedPilot( getDatabase());

      char tmp[256];
      sprintf( tmp, "Pilot_%d", i+1);
      p->setName( tmp);

      sprintf( tmp, "Johnny_%d", i+1);
      p->setFirstName( tmp);

      p->setPoints(i+1);
      p->setLicenseID(i+1);
                
      p->store( eyedb::FullRecurs);

      if ( commitInterval > 0  &&  ++commitCount >= commitInterval ) {
	commitCount = 0;
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

void Bahrain::query( int selectCount, const char *queryString)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      char tmp[256];
      sprintf( tmp, queryString, i+1);

      eyedb::OQL q(getDatabase(), queryString);
      eyedb::ObjectArray arr;

      q.execute(arr);

      int s = 0;
      for (int i = 0; i < arr.getCount(); i++) {
 	IndexedPilot *p = IndexedPilot_c(arr[i]);
	s += p->getPoints();
      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::query_indexed_string( int selectCount)
{
  query( selectCount, "select p from IndexedPilot as p where p.name=\"Pilot_%d\"");
}

void Bahrain::query_string( int selectCount)
{
  query( selectCount, "select p from IndexedPilot as p where p.firstName=\"Johnny_%d\"");
}

void Bahrain::query_indexed_int( int selectCount)
{
  query( selectCount, "select p from IndexedPilot as p where p.licenseID=%d");
}

void Bahrain::query_int( int selectCount)
{
  query( selectCount, "select p from IndexedPilot as p where p.points=%d");
}

void Bahrain::update( int updateCount)
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select p from IndexedPilot as p");
    eyedb::ObjectArray arr;

    q.execute(arr);

    for (int i = 0; i < updateCount; i++) {
      IndexedPilot *p = IndexedPilot_c(arr[i]);

      string name = p->getName();
      for ( int i = 0; i < name.length(); i++) {
	name[i] = toupper(name[i]);
      }
      p->setName( name );
      p->store( eyedb::FullRecurs);

    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::remove()
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select p from IndexedPilot as p");
    eyedb::ObjectArray arr;

    q.execute(arr);

    int s = 0;
    for (int i = 0; i < arr.getCount(); i++) {
      IndexedPilot *p = IndexedPilot_c(arr[i]);
      p->remove();
      s += 5;
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Bahrain::run()
{
  vector<int> objects;
  vector<int> selects;
  vector<int> updates;
  vector<int> commitinterval;

  getProperties().getIntProperty( "bahrain.objects", objects);
  getProperties().getIntProperty( "bahrain.selects", selects);
  getProperties().getIntProperty( "bahrain.updates", updates);
  getProperties().getIntProperty( "bahrain.commitinterval", commitinterval);

  getResult().addHeader( "objects");
  getResult().addHeader( "selects");
  getResult().addHeader( "updates");
  getResult().addHeader( "commitinterval");

  for (int i = 0; i < objects.size(); i++) {

    getResult().addValue( objects[i]);
    getResult().addValue( selects[i]);
    getResult().addValue( updates[i]);
    getResult().addValue( commitinterval[i]);

    getStopwatch().start();

    write( objects[i], commitinterval[i]);
    getStopwatch().lap( "write");

    query_indexed_string( selects[i]);
    getStopwatch().lap( "query_indexed_string");

    query_string( selects[i]);
    getStopwatch().lap( "query_string");

    query_indexed_int( selects[i]);
    getStopwatch().lap( "query_indexed_int");

    query_int( selects[i]);
    getStopwatch().lap( "query_int");

    update( updates[i]);
    getStopwatch().lap( "update");

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
  Bahrain b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  polepos initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  r.setColumnWidth( 25);
  r.report(b);
}
