#include <sstream>
#include <vector>
#include <ctype.h>
#include "polepos.h"
#include "imola.h"

using namespace std;

void Imola::storePilot( int i, int selectCount, int commitInterval) throw( eyedb::Exception)
{
  IndexedPilot *p = new IndexedPilot( getDatabase());

  char tmp[256];
  sprintf( tmp, "Pilot_%d", i+1);
  p->setName( tmp);

  sprintf( tmp, "Johnny_%d", i+1);
  p->setFirstName( tmp);
                
  p->store( eyedb::FullRecurs);
	
  if (isCommitPoint(i, commitInterval)) 
    getDatabase()->transactionCommit();

  if ( i < selectCount)
    oids.push_back( p->getOid());
}

bool Imola::isCommitPoint( int i, int commitInterval)
{
  return commitInterval > 0 && i % commitInterval == 0;
}

void Imola::store( int objectCount, int selectCount, int commitInterval)
{
  try {
    getDatabase()->transactionBegin();
            
    for ( int i = 0; i < objectCount; i++ )
      storePilot(i, selectCount, commitInterval);
                
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Imola::retrieve()
{
  try {
    getDatabase()->transactionBegin();
    
    int s = 0;
    for ( int i = 0; i < oids.size(); i++) {
      //IndexedPilot *p = 
    }

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Imola::run()
{
  vector<int> objects;
  vector<int> selects;
  vector<int> commitinterval;

  getProperties().getIntProperty( "bahrain.objects", objects);
  getProperties().getIntProperty( "bahrain.selects", selects);
  getProperties().getIntProperty( "bahrain.commitinterval", commitinterval);

  getResult().addHeader( "objects");
  getResult().addHeader( "selects");
  getResult().addHeader( "commitinterval");

  for (int i = 0; i < objects.size(); i++) {

    getResult().addValue( objects[i]);
    getResult().addValue( selects[i]);
    getResult().addValue( commitinterval[i]);

    getStopwatch().start();

    store( objects[i], selects[i], commitinterval[i]);
    getStopwatch().lap( "store");

    retrieve();
    getStopwatch().lap( "retrieve");

    getStopwatch().stop();

    getResult().addLaps( getStopwatch().getLaps());

    getStopwatch().reset();

    getResult().next();
  }
}

int main(int argc, char *argv[])
{
  Imola b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  polepos initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  r.setColumnWidth( 25);
  r.report(b);
}
