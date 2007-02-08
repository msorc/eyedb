
/*
 * objgarb_3.cc
 *
 * Eric Viara EyeDB Project
 */

#include <iostream>
#include "schema.h"
#include <typeinfo>

using namespace eyedb;

static int usage(const char *prog) {
  std::cerr << "usage: " << prog << " <dbname>" << std::endl;
  return 1;
}

#define USE_STACK

static Stage *perform(eyedb::Database &db)
{
#ifdef USE_STACK
  Supercontig sctg(&db);
  printf("sctg %p\n", &sctg);
  sctg.setRegionsCount(3);
  Region *r = sctg.getRegions(0);
  Sequence seq(&db);
  printf("seq %p\n", &seq);
  seq.setName("coucou");
  r->setSequence(&seq);
  sctg.store(RecMode::FullRecurs);

  Stage stage(&db);
  printf("stage %p\n", &stage);
  stage.setGroupsCount(10);
  stage.getGroups(0)->getSctgsColl()->insert(&sctg);
  GroupName gn(&db);
  printf("gn %p\n", &gn);
  gn.setName("hello les lapins");
  stage.getGroups(0)->setName(&gn);
  stage.store(RecMode::FullRecurs);

  Stage *stage2 = new Stage(&db);
  printf("stage %p\n", stage2);
  stage2->setGroupsCount(10);
  stage2->getGroups(0)->getSctgsColl()->insert(&sctg);
  gn.setName("ouin");
  stage2->getGroups(0)->setName(&gn);
  stage2->store(RecMode::FullRecurs);

  stage2->release();
  return stage2;
#else
  Supercontig *sctg = new Supercontig(&db);
  sctg->setRegionsCount(3);
  Region *r = sctg->getRegions(0);
  Sequence *seq = new Sequence(&db);
  seq->setName("coucou");
  r->setSequence(seq);
  sctg->store(RecMode::FullRecurs);

  seq->release();
    
  Stage *stage = new Stage(&db);
  stage->setGroupsCount(10);
  stage->getGroups(0)->getSctgsColl()->insert(sctg);
  GroupName *gn = new GroupName(&db);
  gn->setName("hello les lapins");
  stage->getGroups(0)->setName(gn);
  gn->release();
  stage->store(RecMode::FullRecurs);
  //stage->release();

  sctg->release();
  return stage;
#endif
}

void breakit() {printf("breakit\n");}

class AddObjTrigger : public gbxObserver::AddObjectTrigger {
  eyedb::ObjectObserver &observer;
public:
  AddObjTrigger(eyedb::ObjectObserver &observer) : observer(observer) {}
  void operator()(gbxObject *o) {
    std::cerr << "adding object " << (void *)o << " " << observer.getObjectCount() << '\n';
  }
};

class RemoveObjTrigger : public gbxObserver::RemoveObjectTrigger {
  eyedb::ObjectObserver &observer;
public:
  RemoveObjTrigger(eyedb::ObjectObserver &observer) : observer(observer) {}
  void operator()(gbxObject *o) {
    const std::type_info &t = typeid(*o);
    std::cerr << "removing object " << (void *)o << " " << observer.getObjectCount() << '\n';
  }
};

class MyObserver {

  eyedb::ObjectObserver observer;

public:
  MyObserver() {
    //observer.setAddObjectTrigger(new AddObjTrigger(observer));
    //observer.setRemoveObjectTrigger(new RemoveObjTrigger(observer));
  }

  ~MyObserver() {
    std::vector<Object *> v;
    observer.getObjects(v);
    printf("EyeDB Object Count %u\n", v.size());
    std::vector<Object *>::iterator begin = v.begin();
    std::vector<Object *>::iterator end = v.end();
    while (begin != end) {
      const std::type_info &t = typeid(**begin);
      std::clog << (void *)*begin << " " << t.name() << " " << (*begin)->getRefCount() << std::endl;
      ++begin;
    }
  }

};

class SmartObject {
protected:
  Object *o;

public:
  SmartObject(Object *o) {
    this->o = o;
    std::cout << "SmartObject::SmartObject(" << (void *)o << std::endl;
    if (o)
      o->reserve();
  }

  SmartObject(const SmartObject &so) {
    o = 0;
    std::cout << "SmartObject::SmartObject clone(" << (void *)o << std::endl;
    *this = so;
  }

  SmartObject& operator=(const SmartObject &so) {
    std::cout << "SmartObject::operator=(" << (void *)o << std::endl;
    if (o)
      o->release();
    o = so.o;
    o->reserve();
  }

  Object *getObject() {return o;};
  Object *operator->() {return o;};

  ~SmartObject() {
    std::cout << "SmartObject::~SmartObject(" << (void *)o << std::endl;
    if (o)
      o->release();
  }
};

class SmartC : public SmartObject {

public:
  SmartC(C *c) : SmartObject(c) { }

  //  C *getC() {return dynamic_cast<C *>(o);};
  C *operator->() {return dynamic_cast<C *>(o);};
};

SmartC perform_2(Database &db)
{
  D *d = new D(&db);
  SmartObject sd = d;
  d->release(); // could be avoid in case of information is given from API

  SmartC lc = d->getLc();
  lc->getI();
  d->store();
  return lc;
}

#include <eyedbsm/xdr.h>

int
main(int argc, char *argv[])
{
  MyObserver observer;
 
  schema initializer(argc, argv);
 
  if (argc < 2) {
    return usage(argv[0]);
  }

  Oid xoid1("667957760.512.402208:oid");
  Oid hoid1;
  eyedbsm::x2h_oid(hoid1.getOid(), xoid1.getOid());
  std::cout << hoid1 << std::endl;

  Oid xoid2("2322269184.0.3548067:oid");
  Oid hoid2;
  eyedbsm::x2h_oid(hoid2.getOid(), xoid2.getOid());
  std::cout << hoid2 << std::endl;

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn(true);
    schemaDatabase db(&conn, argv[1], eyedb::Database::DBRW);

    db.transactionBegin();
    SmartC lc = perform_2(db);
    db.transactionCommit();
  }
  catch(eyedb::Exception &e) {
    std::cerr << "** Error ** " << e << std::endl;
    return 1;
  }


  return 0;
}
