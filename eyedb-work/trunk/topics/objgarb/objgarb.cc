
/*
 * objgarb.cc
 *
 * Eric Viara EyeDB Project
 */

#include <iostream>
#include "schema.h"

using namespace eyedb;

static int usage(const char *prog) {
  std::cerr << "usage: " << prog << " <dbname>" << std::endl;
  return 1;
}

class AddObjTrigger : public gbxObserver::AddObjectTrigger {
public:
  void operator()(gbxObject *o) {
    printf("adding object %p\n", o);
  }
};

class RemoveObjTrigger : public gbxObserver::RemoveObjectTrigger {
public:
  void operator()(gbxObject *o) {
    printf("removing object %p %s\n", o, dynamic_cast<Object *>(o)->getClass()->getName());
  }
};

int
main(int argc, char *argv[])
{
  eyedb::ObjectObserver *observer;
  observer = new eyedb::ObjectObserver();
  observer->setAddObjectTrigger(new AddObjTrigger());
  observer->setRemoveObjectTrigger(new RemoveObjTrigger());
  eyedb::init(argc, argv);
  schema::init();

  if (argc < 2) {
    return usage(argv[0]);
  }

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn;

    conn.open();

    schemaDatabase db(argv[1]);
    db.open(&conn, eyedb::Database::DBRW);

    db.transactionBegin();

    int objcnt = gbxObject::getObjectCount();

    //    gbxObserver gbx_observer;
    for (int n = 0; n < 10; n++) {
      C *c = new C(&db);

      D *d = new D(&db);
      d->setC(c);
      CollSet *set = d->getCSet1Coll();
      C *c1 = new C(&db);
      set->insert(c1);
      c1->release();

      c->release();
      d->store(RecMode::FullRecurs);
      d->release();
    }

    Supercontig *s = new Supercontig(&db);
    s->setRegionsCount(3);
    Region *r = s->getRegions(0);
    Sequence *seq = new Sequence(&db);
    seq->setName("coucou");
    r->setSequence(seq);
    s->store(RecMode::FullRecurs);
    std::cout << "gbx_objcnt: " << (gbxObject::getObjectCount() - objcnt) << std::endl;
#if 0
    {
      std::vector<gbxObject *> v;
      gbx_observer.getObjects(v);
      std::vector<gbxObject *>::iterator begin = v.begin();
      std::vector<gbxObject *>::iterator end = v.end();
      while (begin != end) {
	++begin;
      }
    }
#else
    /*
    {
      std::vector<Object *> v;
      observer.getObjects(v);
      printf("eyedb_objcnt %u\n", v.size());
      std::vector<Object *>::iterator begin = v.begin();
      std::vector<Object *>::iterator end = v.end();
      while (begin != end) {
	printf("%p %s %d\n", *begin, (*begin)->getClass()->getName(),
	       (*begin)->isLocked());
	++begin;
      }
    }
    */
#endif

    db.transactionCommit();

    db.close();
  }
  catch(eyedb::Exception &e) {
    std::cerr << "** Error ** " << e << std::endl;
    return 1;
  }

  schema::release();
  eyedb::release();

  std::vector<Object *> v;
  observer->getObjects(v);
  printf("eyedb_objcnt %u\n", v.size());
  std::vector<Object *>::iterator begin = v.begin();
  std::vector<Object *>::iterator end = v.end();
  while (begin != end) {
    printf("%p %s %d\n", *begin, (*begin)->getClass()->getName(),
	   (*begin)->isLocked());
    ++begin;
  }

  return 0;
}
