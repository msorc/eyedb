
/*
 * collinit.cc
 *
 *
 */

#include <iostream>
#include "schema.h"
#include <eyedblib/strutils.h>

using namespace eyedb;

static int usage(const char *prog) {
  std::cerr << "usage: " << prog << " <dbname>" << std::endl;
  return 1;
}

#define INIT_COLL
#define REINIT_COLL
//#define COPY_COLL

static void perform(Database *db)
{
  Class *I_cls = db->getSchema()->getClass("I");
  CollSet *set = 0;
  if (I_cls) {
    set = new CollSet(db, "", I_cls);
    I *i;
    for (int n = 0; n < 100; n++) {
      i = new I(db);
      i->setI(n+1);
      i->setName(("copy " + str_convert(n)).c_str());
      i->store();
      set->insert(i->getOid());
    }
    set->store();
    std::cout << set << std::endl;
  }

  C *c = new C(db);
  c->setId(1);

  std::cout << "literal_oid0: " << c->getIsColl()->getLiteralOid() << std::endl;
  c->store();

#ifdef INIT_COLL
  for (int n = 0; n < 10; n++) {
    I *i = new I(db);
    i->setI(n + 1);
    i->setName(("orig " + str_convert(n)).c_str());
    i->store();
    c->addToIsColl(i->getOid());
    //c->addToIsColl(i);
  }
  c->store();
#endif
  std::cout << "literal_oid: " << c->getIsColl()->getLiteralOid() << std::endl;
  std::cout << c << std::endl;

#ifdef REINIT_COLL
  c->setIsColl(set);
  //c->getIsColl()->setLiteralOid(set->getOid());
  c->store();
  std::cout << "literal_oid after reinit: " << c->getIsColl()->getLiteralOid() << std::endl;
  std::cout << c << std::endl;
#endif

#ifdef COPY_COLL
  C *c1 = new C(db);
  c1->setId(2);

  std::cout << "literal_oid after copy: " << c1->getIsColl()->getLiteralOid() << std::endl;
  c1->store();

  c1->setIsColl(c->getIsColl());

  c1->store();
#endif
}

int main(int argc, char *argv[])
{
  schema initializer(argc, argv);

  if (argc < 2) {
    return usage(argv[0]);
  }

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn(true);

    schemaDatabase db(&conn, argv[1], eyedb::Database::DBRW);

    db.transactionBegin();

    printf("here we go...\n");
    C *c1 = new C(&db);
    I *i1 = c1->getI1();
    C *c2 = new C(&db);
    I *i2 = c2->getI1();
    *c1 = *c2;
    if (false)
      perform(&db);

    db.transactionCommit();

    db.close();
  }
  catch(eyedb::Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }


  return 0;
}
