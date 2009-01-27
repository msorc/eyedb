#include <string>
#include "odl_sample.h"

using namespace std;

class InsertPacket {
public:
  InsertPacket( string dbName, int nPackets, int nRecordsPerPacket, int nPacketsPerTransaction)
    : dbName(dbName), 
      nPackets(nPackets), 
      nRecordsPerPacket(nRecordsPerPacket), 
      nPacketsPerTransaction(nPacketsPerTransaction)
  {
  }

  void prepare() throw( eyedb::Exception);
  void finish() throw( eyedb::Exception);

  void run() throw( eyedb::Exception);

private:
  Packet *createPacket( int n) throw( eyedb::Exception);
  Record *createRecord( int n) throw( eyedb::Exception);
  base_field *createVegetable_field() throw( eyedb::Exception);
  base_field *createMeat_field() throw( eyedb::Exception);

  eyedb::Database *getDatabase() { return database; }

  eyedb::Connection *conn;
  eyedb::Database *database;

  string dbName;
  int nPackets;
  int nRecordsPerPacket;
  int nPacketsPerTransaction;
};

void InsertPacket::prepare() throw( eyedb::Exception)
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  conn = new eyedb::Connection( true);

  eyedb::Database::OpenFlag flags;
  flags = eyedb::Database::DBRWLocal;

  database = new odl_sampleDatabase( conn, dbName.c_str(), flags);
}

void InsertPacket::finish() throw( eyedb::Exception)
{
  database->close();
  conn->close();
}

Packet *InsertPacket::createPacket( int n) throw( eyedb::Exception)
{
  Packet *packet = new Packet( getDatabase());

  //packet->setDay( ?);

  char tmp[512];
  sprintf( tmp, "Packet_%d", n);
  packet->setName( tmp);

  for (int n = 0; n < nRecordsPerPacket; n++) {
    Record *record = createRecord( n);

    packet->addToRecordsColl( record);
  }

  return packet;
}

#define nStatusPerRecord 3

Record *InsertPacket::createRecord( int n) throw( eyedb::Exception)
{
  Record *record = new Record( getDatabase());

  char tmp[512];
  sprintf( tmp, "Record_%d", random());
  record->setName( tmp);

  record->addToFieldColl( createVegetable_field());
  record->addToFieldColl( createMeat_field());

  record->setStatusCount( nStatusPerRecord);
  for ( int i = 0; i < nStatusPerRecord; i++)
    record->setStatus( i, good);

  return record;
}

base_field *InsertPacket::createVegetable_field() throw( eyedb::Exception)
{
  vegetable_field *field = new vegetable_field( getDatabase());

  field->setId( one);
  field->setVegetableType( potatoe);

  return field;
}

base_field *InsertPacket::createMeat_field() throw( eyedb::Exception)
{
  meat_field *field = new meat_field( getDatabase());

  field->setId( two);
  field->setMeatType( beef);

  return field;
}

void InsertPacket::run() throw( eyedb::Exception)
{
  getDatabase()->transactionBegin();

  for ( int n = 0; n < nPackets; n++) {
    Packet *packet = createPacket( n);

    packet->store( eyedb::FullRecurs);
      
    if (n % nPacketsPerTransaction == nPacketsPerTransaction - 1) {
      getDatabase()->transactionCommit();
      getDatabase()->transactionBegin();
    }
  }

  getDatabase()->transactionCommit();
}

int main( int argc, char **argv)
{
  odl_sample initializer(argc, argv);

  if (argc < 5) {
    cerr << "Usage: " << argv[0] << " database nPackets nRecordsPerPacket nPacketsPerTransaction" << endl;
    exit(1);
  }

  string dbName = argv[1];
  int nPackets, nRecordsPerPacket, nPacketsPerTransaction;
  sscanf( argv[2], "%d", &nPackets);
  sscanf( argv[3], "%d", &nRecordsPerPacket);
  sscanf( argv[4], "%d", &nPacketsPerTransaction);

  try {
    InsertPacket ip( dbName, nPackets, nRecordsPerPacket, nPacketsPerTransaction);
    ip.prepare();
    ip.run();
    ip.finish();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}
