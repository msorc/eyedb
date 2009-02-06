
// to compile:
// PKG_CONFIG_PATH=~/eyedb-install-pro/lib/pkgconfig make

#include "schema.h"
#include <iostream>

using namespace eyedb;
using namespace std;

//#define USE_OID

int main(int argc, char *argv[])
{
  eyedb::init(argc, argv);

  schema::init();

  if (argc != 5) {
    std::cerr << "usage: " << argv[0] << " DBNAME COUNT ARR_C_COUNT BAG_C_COUNT_COUNT\n";
    return 1;
  }

  const char *dbname = argv[1];
  int count = atoi(argv[2]);
  int arr_c_count = atoi(argv[3]);
  int bag_c_count = atoi(argv[4]);

  Exception::setMode(Exception::ExceptionMode);

  //putenv("NO_CLASS_CHECK=1");

  try {
    Connection conn;

    conn.open();

    schemaDatabase db(dbname);
    db.open(&conn, Database::DBRWLocal);
    
    std::cout << "Creating " << count << " C instances with " << arr_c_count << " C array items, " << bag_c_count << " C bag items\n";
    db.transactionBegin();
    for (int n = 0; n < count; n++) {
      C *c = new C(&db);
      c->setC(n);

      C *c1 = new C(&db);
      c1->setC(2 * n + 1);

      if (bag_c_count) {
#ifdef USE_OID
	if (c1->getOid().isInvalid()) {
	  c1->store();
	}
#endif
	for (int m = 0; m < bag_c_count; m++) {
#ifdef USE_OID
	  c->addToBagCColl(c1->getOid());
#else
	  c->addToBagCColl(c1);
#endif
	}
      }

      if (arr_c_count) {
#ifdef USE_OID
	if (c1->getOid().isInvalid()) {
	  c1->store();
	}
#endif
	c->setArrCCount(arr_c_count);
	for (int m = 0; m < arr_c_count; m++) {
#ifdef USE_OID
	  c->setArrCOid(m, c1->getOid());
#else
	  c->setArrC(m, c1);
#endif
	}
      }

      c->store(eyedb::RecMode::FullRecurs);
      c->release();
      c1->release();
    }

    db.transactionCommit();
  }

  catch(Exception &e) {
    cerr << argv[0] << ": " << e << endl;
    return 1;
  }

  return 0;
}

