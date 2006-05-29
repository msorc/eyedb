
#include "schema.h"
#include <assert.h>

#define TRY_STORE

using namespace eyedb;
using namespace std;

static void
bug_coll_elements(Database &db)
{
#ifdef TRY_STORE
  //  db.registerObjects(True);
  db.storeOnCommit(True);
#endif
  OidArray oid_arr;
  ObjectArray obj_arr;
  A *a = new A(&db);

  a->addToSetB1Coll(new B(&db));
  a->addToSetB1Coll(new B(&db));

  a->getSetB1Coll()->getElements(obj_arr);
  assert(obj_arr.getCount() == 2);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

  assert(a->addToSetB1Coll(0));

#ifndef TRY_STORE
  a->store(RecMode::FullRecurs);
#endif

  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

  B *b = new B(&db);

  a->addToSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 3);
  assert(a->getSetB1Coll()->getCount() == 3);

  a->rmvFromSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

#ifndef TRY_STORE
  a->store(RecMode::FullRecurs);
#endif

  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

#ifdef TRY_STORE
  //  db.storeObjects();
#endif
}

static void
bug_coll_empty(Database &db)
{
  OidArray oid_arr;
  A *a = new A(&db);
  B *b = new B(&db);

  a->addToSetB1Coll(b);
  a->addToSetB1Coll(new B(&db));
  a->addToSetB1Coll(new B(&db));
  // 3 elements added dans le cache (o != NULL, oid == NULL)
  // 0 element dans le read_cache
  a->getSetB1Coll()->getElements(oid_arr);
  // 3 elements added dans le cache (o != NULL, oid == NULL)
  // 3 element dans le read_cache (o != NULL, oid == NULL)
  assert(oid_arr.getCount() == 3);
  assert(a->getSetB1Coll()->getCount() == 3);

  a->getSetB1Coll()->empty(); 
  // 3 elements suppress dans le cache (o != NULL, oid == NULL)
  // 0 element dans le read_cache

  a->getSetB1Coll()->getElements(oid_arr);
  // 3 elements suppress dans le cache (o != NULL, oid == NULL)
  // 0 element dans le read_cache
  assert(oid_arr.getCount() == 0);
  assert(a->getSetB1Coll()->getCount() == 0);

  printf("continue> ");
  getchar();
  a->addToSetB1Coll(b);
  // 3 elements suppress dans le cache + 1 element added (o != NULL, oid == NULL)
  // 0 element dans le read_cache

  a->getSetB1Coll()->getElements(oid_arr);
  // 3 elements suppress dans le cache + 1 element added
  // 0 element dans le read_cache

  assert(oid_arr.getCount() == 1);
  assert(a->getSetB1Coll()->getCount() == 1);

  b = new B(&db);
  a->addToSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

  a->getSetB1Coll()->store(RecMode::FullRecurs);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 2);
  assert(a->getSetB1Coll()->getCount() == 2);

  a->getSetB1Coll()->empty();
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 0);
  assert(a->getSetB1Coll()->getCount() == 0);

  printf("continue 2> ");
  getchar();
  a->addToSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  assert(oid_arr.getCount() == 1);
  assert(a->getSetB1Coll()->getCount() == 1);
}

static void
bug_coll_cnt_at(Database &db)
{
  OidArray oid_arr;

  A *a = new A(&db);
  B *b = new B(&db);
  a->addToSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  printf("#1 oid_arr count %d\n", oid_arr.getCount());
  a->getSetB1Coll()->getElements(oid_arr);
  printf("#2 oid_arr count %d\n", oid_arr.getCount());
  a->addToSetB1Coll(new B(&db));

  CollectionIterator i1(a->getSetB1Coll());
  Object *b1;
  while (i1.next(b1))
    cout << "##1 " << (void *)b1 << " ";
  /*
  for (int i = 0; i < a->getSetB1Count(); i++)
    cout << "##1 " << (void *)a->getSetB1At(i) << " ";
  */

  cout << endl;

  b = new B(&db);
  a->addToSetB1Coll(b);
  a->getSetB1Coll()->getElements(oid_arr);
  printf("#1 oid_arr count %d\n", oid_arr.getCount());
  a->getSetB1Coll()->store(RecMode::FullRecurs);
  a->getSetB1Coll()->empty();
  a->getSetB1Coll()->getElements(oid_arr);
  printf("#2 oid_arr count %d\n", oid_arr.getCount());
  a->addToSetB1Coll(b);

  CollectionIterator i2(a->getSetB1Coll());
  while (i2.next(b1))
    cout << "##2 " << (void *)b1 << " ";
  /*
  for (int i = 0; i < a->getSetB1Count(); i++)
    cout << "##2 " << (void *)a->getSetB1At(i) << " ";
  */

  cout << endl;
}

static void
display_coll_elements(const char *when, Database &db, A *a)
{
  OidArray oid_arr;
  ValueArray val_arr;

  a->getArrB4Coll()->getElements(oid_arr);

  printf("%s bottom %d and top %d\n", when, a->getArrB4Coll()->getBottom(),
	 a->getArrB4Coll()->getTop());

  assert(oid_arr.getCount() == 3);

  printf("objat[0] = %p\n", a->retrieveArrB4At(0));
  printf("objat[1] = %p\n", a->retrieveArrB4At(1));
  printf("objat[2] = %p\n", a->retrieveArrB4At(2));
  printf("objat[3] = %p\n", a->retrieveArrB4At(3));
  printf("objat[4] = %p\n", a->retrieveArrB4At(4));
  printf("objat[5] = %p\n", a->retrieveArrB4At(5));

  printf("oidat[0] = %s\n", a->retrieveArrB4OidAt(0).toString());
  printf("oidat[1] = %s\n", a->retrieveArrB4OidAt(1).toString());
  printf("oidat[2] = %s\n", a->retrieveArrB4OidAt(2).toString());
  printf("oidat[3] = %s\n", a->retrieveArrB4OidAt(3).toString());
  printf("oidat[4] = %s\n", a->retrieveArrB4OidAt(4).toString());
  printf("oidat[5] = %s\n", a->retrieveArrB4OidAt(5).toString());

  a->getArrB4Coll()->getElements(val_arr, True);

  printf("val_arr.getCount() == %d\n", val_arr.getCount());
    //assert(val_arr.getCount() == 6);
  for (int i = 0; i < 10; i++)
    {
      Oid oid;
      Object *o;
      a->getArrB4Coll()->retrieveAt(i, oid);
      printf("item_oid[%d] = %s\n", i, oid.toString());
      a->getArrB4Coll()->retrieveAt(i, o);
      printf("item_obj[%d] = %p\n", i, o);
    }
  
}

static void
bug_coll_array(Database &db)
{
  A *a = new A(&db);
  a->setInArrB4CollAt(0, new B(&db));
  a->setInArrB4CollAt(3, new B(&db));
  a->setInArrB4CollAt(4, new B(&db));

  display_coll_elements("first", db, a);
  a->store(RecMode::FullRecurs);
  display_coll_elements("middle", db, a);

  db.transactionCommit();

  db.transactionBegin();
  Object *o;
  db.loadObject(a->getOid(), o);
  a = A_c(o);
  display_coll_elements("last", db, a);
}

int
main(int argc, char *argv[])
{
  // initializing the EyeDB layer
  eyedb::init(argc, argv);

  // initializing the person package
  schema::init();

  if (argc != 2)
    {
      fprintf(stderr, "usage: %s <dbname>\n",
	      argv[0]);
      return 1;
    }

  const char *dbname = argv[1];

  Exception::setMode(Exception::ExceptionMode);

  try {
    Connection conn;

    conn.open();

    Database db(dbname);
    db.open(&conn, (getenv("EYEDBLOCAL") ?
		    Database::DBRWLocal :
		    Database::DBRW));
    
    db.transactionBegin();

    /*
    bug_coll_elements(db);
    bug_coll_empty(db);
    bug_coll_cnt_at(db);
    */
    bug_coll_array(db);

    db.transactionCommit();
  }

  catch(Exception &e) {
    cerr << argv[0] << ": " << e;
    return 1;
  }

  return 0;
}
