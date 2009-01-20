#include <sstream>
#include <vector>
#include "polepos.h"
#include "melbourne.h"

using namespace std;

void Melbourne::write( int objectCount, int commitInterval)
{
  try {
    getDatabase()->transactionBegin();
            
    int commitCount = 0;

    for ( int i = 0; i < objectCount; i++ ){
      Pilot *p = new Pilot( getDatabase());

      char tmp[256];
      sprintf( tmp, "Pilot_%d", i+1);
      p->setName( tmp);

      sprintf( tmp, "Herkules_%d", i+1);
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

void Melbourne::read( int selectCount)
{
  try {
    getDatabase()->transactionBegin();

    for (int i = 0; i < selectCount; i++) {
      eyedb::OQL q(getDatabase(), "select p from Pilot as p");
      eyedb::ObjectArray arr;

      q.execute(arr);

      int s = 0;
      for (int i = 0; i < arr.getCount(); i++) {
 	Pilot *p = Pilot_c(arr[i]);
	s += p->getPoints();
      }
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Melbourne::read_hot( int selectCount)
{
  read( selectCount);
}

void Melbourne::remove()
{
  try {
    getDatabase()->transactionBegin();

    eyedb::OQL q(getDatabase(), "select p from Pilot as p");
    eyedb::ObjectArray arr;

    q.execute(arr);

    int s = 0;
    for (int i = 0; i < arr.getCount(); i++) {
      Pilot *p = Pilot_c(arr[i]);
      p->remove();
      s += 5;
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Melbourne::run()
{
  vector<int> objects;
  vector<int> selects;
  vector<int> commitinterval;

  getProperties().getIntProperty( "melbourne.objects", objects);
  getProperties().getIntProperty( "melbourne.selects", selects);
  getProperties().getIntProperty( "melbourne.commitinterval", commitinterval);
  
  getResult().addHeader( "objects");
  getResult().addHeader( "selects");
  getResult().addHeader( "commitinterval");

  for (int i = 0; i < objects.size(); i++) {

    getResult().add( objects[i]);
    getResult().add( selects[i]);
    getResult().add( commitinterval[i]);

    getStopwatch().start();

    write( objects[i], commitinterval[i]);
    getStopwatch().lap( "write");

    read( selects[i]);
    getStopwatch().lap( "read");

    read_hot( selects[i]);
    getStopwatch().lap( "read_hot");

    remove();
    getStopwatch().lap( "remove");

    getStopwatch().stop();

    getResult().add( getStopwatch().getLaps());

    getStopwatch().reset();

    getResult().next();
  }
}

int main(int argc, char *argv[])
{
  Melbourne b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  polepos initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  r.setColumnWidth( 16);
  r.report(b);
}
