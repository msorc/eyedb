#include <eyedb/eyedb.h>
#include "barcelona.h"
#include "polepos.h"

void Barcelona::write()
{
  try {
    getDatabase()->transactionBegin();
            
    //    int count = setup()->getObjectCount(); 
    int count = 10000; 

    for ( int i = 0; i < count; i++) {
      B4 *b4 = new B4( getDatabase());

      b4->setB0( i+1);
      b4->setB1( i+1);
      b4->setB2( i+1);
      b4->setB3( i+1);
      b4->setB4( i+1);

      b4->store( eyedb::FullRecurs);
    }
            
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Barcelona::read()
{
}

void Barcelona::query()
{
}

void Barcelona::destroy()
{
}

void Barcelona::run()
{
  write();
  lap( "write");
  read();
  lap( "read");
  query();
  lap( "query");
  destroy();
  lap( "destroy");
}
