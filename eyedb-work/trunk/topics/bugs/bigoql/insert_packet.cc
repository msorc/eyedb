#include <string>
#include "properties.h"
#include "odl_sample.h"

using namespace std;

class InsertPacket {
public:
  InsertPacket( const char *propertiesFile)
  {
    properties.load( propertiesFile);
  }

  void prepare() throw( eyedb::Exception);
  void finish() throw( eyedb::Exception);

  void run() throw( eyedb::Exception);

private:
  Packet *createPacket( int n) throw( eyedb::Exception);
#if 0
  Record *createRecord() throw( eyedb::Exception);
  base_field *createDerived_field() throw( eyedb::Exception);
#endif

  eyedb::Database *getDatabase() { return database; }
  eyedb::benchmark::Properties &getProperties() { return properties; }

  eyedb::Connection *conn;
  eyedb::Database *database;

  eyedb::benchmark::Properties properties;
};

void InsertPacket::prepare() throw( eyedb::Exception)
{
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  conn = new eyedb::Connection( true);

  eyedb::Database::OpenFlag flags;
  flags = eyedb::Database::DBRWLocal;

  string dbName;
  getProperties().getStringProperty( "eyedb.database", dbName);

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

  char tmp[512];
  sprintf( tmp, "Packet_%d", n);
  packet->setName( tmp);

#if 0
  int nRecordsPerPacket;
  getProperties().getIntProperty( "nRecordsPerPacket", nRecordsPerPacket);

  for (int n = 0; n < nRecordsPerPacket; n++) {
    Record *record = createRecord();

    packet->addToRecordsColl( record);
#ifndef USE_INVERSE
    record->setPacket( packet);
#endif
  }
#endif

  return packet;
}

#if 0
Record *InsertPacket::createRecord() throw( eyedb::Exception)
{
  Record *record = new Record( getDatabase());

  char tmp[512];
  sprintf( tmp, "Record_%d", random());
  record->setName( tmp);

  int nFieldsPerRecord;
  getProperties().getIntProperty( "nFieldsPerRecord", nFieldsPerRecord);
  for ( int i = 0; i < nFieldsPerRecord; i++) {
    base_field *field = createDerived_field();
    record->addToFieldsColl( field);
#ifndef USE_INVERSE
    field->setRecord( record);
#endif
  }

  int nStatusPerRecord;
  getProperties().getIntProperty( "nStatusPerRecord", nStatusPerRecord);

  record->setStatusCount( nStatusPerRecord);
  for ( int i = 0; i < nStatusPerRecord; i++)
    record->setStatus( i, good);

  return record;
}
#endif

#if 0
base_field *InsertPacket::createDerived_field() throw( eyedb::Exception)
{
  derived_field *field = new derived_field( getDatabase());

  field->setId( ID2);
  field->setDerivedFieldType( TYPE1);

  return field;
}
#endif

void InsertPacket::run() throw( eyedb::Exception)
{
  getDatabase()->transactionBegin();

  int nPackets;
  int nPacketsPerTransaction;

  getProperties().getIntProperty( "nPackets", nPackets);
  getProperties().getIntProperty( "nPacketsPerTransaction", nPacketsPerTransaction);

  time_t t0;
  time(&t0);
  for ( int n = 0; n < nPackets; n++) {
    Packet *packet = createPacket( n);

    packet->store( eyedb::FullRecurs);
     
    packet->release();

    if (n % nPacketsPerTransaction == nPacketsPerTransaction - 1) {
      getDatabase()->transactionCommit();

      time_t t1;
      time(&t1);
      cout << "Committed " << (n+1) <<  " packets " << (t1-t0) << " seconds" << endl;

      t0 = t1;
      getDatabase()->transactionBegin();
    }
    packet->release();
  }

  getDatabase()->transactionCommit();
}

int main( int argc, char **argv)
{
  odl_sample initializer(argc, argv);

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " PROPERTIES_FILE" << endl;
    exit(1);
  }

  try {
    InsertPacket ip( argv[1]);
    ip.prepare();
    ip.run();
    ip.finish();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}
