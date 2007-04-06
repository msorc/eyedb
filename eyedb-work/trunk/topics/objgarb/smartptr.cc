#include <iostream>
#include "schema.h"

using namespace eyedb;

static int usage(const char *prog) {
  std::cerr << "usage: " << prog << " <dbname>" << std::endl;
  return 1;
}

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
    std::vector<Object *>::iterator begin = v.begin();
    std::vector<Object *>::iterator end = v.end();
    while (begin != end) {
      const std::type_info &t = typeid(**begin);
      std::clog << (void *)*begin << " " << t.name() << " " << (*begin)->getRefCount() << std::endl;
      ++begin;
    }
    printf("EyeDB Object Count %u\n", v.size());
  }
};

static void breakit()
{
}

#define USE_SMARTPTR

#ifdef USE_SMARTPTR

static DPtr perform1(Database *db)
{
  DPtr d = new D(db);

  CPtr lc = d->getLc();

  CPtr lc2 = new C(db);

  CPtr lc3 = lc2;
  lc3 = lc2;

  lc->getI();

  CPtr lcc = d->getLcc1(0);
  lcc->setI(10);

  lcc = d->getLcc1(1);
  lcc->setI(100);

  lcc = d->getLcc1(2);
  lcc->setI(100);

  d->setLcc2Count(20);
  lcc = d->getLcc2(18);

  d->setCc3Count(10);

  CPtr cc = new C(db);
  cc->store();
  /*
  d->setCc3(1, cc.getC());
  d->setCc3(3, cc.getC());
  */
  d->setCc3(1, cc);
  d->setCc3(3, cc);
  CollSetPtr cset = d->getCLsetColl();

#if 1
  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //  cset->insert(cc.getC());
  cset->insert(cc);

  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //cset->insert(cc.getC());
  cset->insert(cc);

  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //cset->insert(cc.getC());
  cset->insert(cc);

#else
  C *cc0 = new C(db);
  cc0->store();
  //std::cout << "inserting " << cc->getPtr()0 << std::endl;
  cset->insert(cc0);

  C *cc1 = new C(db);
  cc1->store();
  //std::cout << "inserting " << cc->getPtr()1 << std::endl;
  cset->insert(cc1);

  C *cc2 = new C(db);
  cc2->store();
  //std::cout << "inserting " << cc->getPtr()2 << std::endl;
  cset->insert(cc2);
#endif

  /*
  */

  d->store();

  ObjectArray obj_arr;
  d->getCLsetColl()->getElements(obj_arr);

  ObjectPtrVector obj_vect;
  d->getCLsetColl()->getElements(obj_vect);
  ObjectPtr poo1 = obj_vect[0];

  //const_cast<Object *>(obj_arr[0])->setMustRelease(false);
  //ObjectPtr poo = const_cast<Object *>(obj_arr[0]);
  ObjectPtr poo = obj_arr[0];

#if 1
  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //cset->insert(cc.getC());
  cset->insert(cc);

  d->getCLsetColl()->getElements(obj_arr);

  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //  cset->insert(cc.getC());
  cset->insert(cc);

  d->getCLsetColl()->getElements(obj_arr);

  cc = new C(db);
  cc->store();
  //std::cout << "inserting " << cc->getPtr() << std::endl;
  //  cset->insert(cc.getC());
  cset->insert(cc);

  d->getCLsetColl()->getElements(obj_arr);
#endif

#if 1
  //printf("beginning val_arr\n");
  ValueArray val_arr;
  d->getCLsetColl()->getElements(val_arr);
  //std::cout << val_arr[0] << std::endl;
  //printf("ending val_arr\n");
#endif

#if 1
  CollectionIterator it(d->getCLsetColl());
  Object *o;
  for (int n = 0; it.next(o); n++) {
    ObjectPtr po = o;
    //printf("n = %d %p refcnt=%d\n", n, o, o->getRefCount());
  }

  CollectionIterator it2(d->getCLsetColl());
  Value v;
  for (int n = 0; it2.next(v); n++) {
    //std::cout << v << std::endl;
    //    ValueArray val_arr(true);
    ValueArray val_arr(false);
    v.toArray(val_arr);
  }


#endif

  return d;
}

static DPtr step1(Database *db)
{
  DPtr d = perform1(db);
  return d;
}

std::vector<ObjectPtr> toObjectPtrArray(ObjectArray &obj_arr)
{
  std::vector<ObjectPtr> v;
  for (int n = 0; n < obj_arr.getCount(); n++)
    v.push_back(obj_arr[n]);
  return v;
}

static void step2(Database *db)
{
  OQL q(db, "select D");
  //ObjectArray obj_arr(true);
  ObjectArray obj_arr(false);
  q.execute(obj_arr);
  //ObjectPtr o = obj_arr[0];
  std::vector<ObjectPtr> v = toObjectPtrArray(obj_arr);
  // si on met obj_arr(true) => pb !

  ObjectArray obj_arr2(true);
  q.execute(obj_arr2);

  //printf("obj_arr.getCount() %d\n", obj_arr.getCount());

  //  ValueArray val_arr(true);
  ValueArray val_arr(false);
  q.execute(val_arr);
  //printf("val_arr.getCount() %d\n", val_arr.getCount());

  OQLIterator it(q);
  Object *o;
  while (it.next(o)) {
    ObjectPtr oo = o;
    // OR:
    //o->release();
  }

  OQLIterator itv(q);
  Value vv;
  while (itv.next(vv)) {
  }
}

static void step3(Database *db)
{
  OQL q(db, "select D");
  ObjectPtrVector obj_vect;
  q.execute(obj_vect);
  //printf("obj_vect.size() %d\n", obj_vect.size());

  q.execute(obj_vect);
  //printf("obj_vect.size() %d\n", obj_vect.size());


  OQLIterator it(q);
  ObjectPtr o;
  ObjectPtr oo;
  while (it.next(o)) {
    ObjectPtr xo;
    db->loadObject(o->getOid(), xo);
  }


#if 0
  //  ValueArray val_arr(true);
  ValueArray val_arr(false);
  q.execute(val_arr);
  //printf("val_arr.getCount() %d\n", val_arr.getCount());

  OQLIterator it(q);
  Object *o;
  while (it.next(o)) {
    ObjectPtr oo = o;
    // OR:
    //o->release();
  }

  OQLIterator itv(q);
  Value vv;
  while (itv.next(vv)) {
  }
#endif
}

//#define CYCLE

static void step4(Database *db)
{
  StagePtr stage = new Stage(db);
  stage->setGroupsCount(10);
  GroupPtr group0 = stage->getGroups(2);
  GroupNamePtr name = new GroupName(db);
  name->setName("hello");
  group0->setName(name);
  CollSetPtr collset = group0->getSctgsColl();
  for (int n = 0; n < 10; n++) {
    SupercontigPtr sctg = new Supercontig(db);
    sctg->setRegionsCount(2);
    RegionPtr r = sctg->getRegions(0);
    SequencePtr seq = new Sequence(db);
    r->setSequence(seq);
    collset->insert(sctg);
    sctg->setCloneLinksCount(20);
    CloneLinkCountPtr clc;

    SupercontigPtr sctg1;
#ifdef CYCLE
    sctg1 = sctg;
#else
    sctg1 = new Supercontig(db);
#endif

    clc = sctg->getCloneLinks(3);
    //std::cout << "setSctg #1\n";
    clc->setSctg(sctg1);

    // eyedb error: attribute clone_links of object 0x63a300 of class Supercontig has been damaged during a prematured release
#if 1
    clc = sctg->getCloneLinks(2);
    //std::cout << "setSctg #2\n";
    clc->setSctg(sctg1);
    clc = sctg->getCloneLinks(18);
    //std::cout << "setSctg #3\n";
    clc->setSctg(sctg1);
    clc = sctg->getCloneLinks(14);
    //std::cout << "setSctg #4\n";
    clc->setSctg(sctg1);
#endif
  }
}

static void step5(Database *db)
{
  DPtr d = new D(db);
  CPtr c = new C(db);
  c->store();
  d->getCLarrayColl()->insertAt(0, c);
  d->store();
  d->getCLarrayColl()->suppressAt(0);

  d->store(); // collection error: item 4010672128.519.514753:oid not found at 0 in 3831.28.410567:oid

  // not necessary to raise the bug
#if 0
  CollSetPtr c_set = new CollSet(db, "", db->getSchema()->getClass("C"));
  c_set->insert(c);
  c_set->store();
  d->getCArraySetColl()->insertAt(0, c_set);
  d->store();
  d->getCArraySetColl()->suppressAt(0);
#endif
}

static void step6(Database *db)
{
  A *ao = 0;
  B *bo = 0;
  {
    APtr a = new A(db); // a.refcnt == 1
    BPtr b = new B(db); // b.refcnt == 1

    ao = a.getA();
    bo = b.getB();

    a->setB1(b); // b.refcnt == 2

    b->setA1(a); // a.refcnt == 2
    b->setA2(a); // a.refcnt == 3

    std::cout << "a.refcnt: " << a->getRefCount() << std::endl;
    std::cout << "b.refcnt: " << b->getRefCount() << std::endl;

    // this line introduces a memory leak:
    a->setB2(b); // b.refcnt == 3

    std::cout << "a.refcnt: " << a->getRefCount() << std::endl;
    std::cout << "b.refcnt: " << b->getRefCount() << std::endl;
  }

  std::cout << "ao->refcnt: " << ao->getPtr() << ": " << ao->getRefCount() << std::endl;
  std::cout << "bo->refcnt: " << bo->getPtr() << ": " << bo->getRefCount() << std::endl;
}
#else
static void step7(Database *db)
{
  A *a = new A(db); // a.refcnt == 1
  B *b = new B(db); // b.refcnt == 1

  a->setB1(b); // b.refcnt == 2

  b->setA1(a); // a.refcnt == 2
  b->setA2(a); // a.refcnt == 3

  std::cout << "a.refcnt: " << a->getRefCount() << std::endl;
  std::cout << "b.refcnt: " << b->getRefCount() << std::endl;

  // this line introduces a memory leak:
#if 1
  a->setB2(b); // b.refcnt == 3
  
  std::cout << "a.refcnt: " << a->getRefCount() << std::endl;
  std::cout << "b.refcnt: " << b->getRefCount() << std::endl;
#endif

  a->release();

  std::cout << "after a release a.refcnt: " << a->getRefCount() << std::endl;
  std::cout << "after a release b.refcnt: " << b->getRefCount() << std::endl;

  b->release();
  // quand #if 1, car le reference count de b == 3, donc == 2 apres le
  // decrRefCount() et donc (!reentrant && refcnt == 1) est faux et donc
  // le managecycle n'est pas appelé
  /*
    je pense que ce refcnt == 1 est faux !
    mais si on l'enleve il faut sans doute changer autre chose
    
    je viens d'enlever ce test: effectivement cela change quelque chose :
    les refcnt apres les release() ne sont plus de 2 mais de 1. Les objets ne
    sont pas releases pour autant !
  */

  std::cout << "after b release a.refcnt: " << a->getRefCount() << std::endl;
  std::cout << "after b release b.refcnt: " << b->getRefCount() << std::endl;
}
#endif

int
main(int argc, char *argv[])
{
  MyObserver observer;
 
  schema initializer(argc, argv);
 
  if (argc < 2) {
    return usage(argv[0]);
  }
  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn(true);
    schemaDatabase db(&conn, argv[1], eyedb::Database::DBRWLocal);

#ifdef USE_SMARTPTR
    db.transactionBegin();
    step1(&db);
    db.transactionCommit();

    db.transactionBegin();
    step2(&db);
    db.transactionCommit();

    db.transactionBegin();
    step3(&db);
    db.transactionCommit();

    db.transactionBegin();
    step4(&db);
    db.transactionCommit();

    db.transactionBegin();
    step5(&db);
    db.transactionCommit();

    db.transactionBegin();
    step6(&db);
    db.transactionCommit();
#else
    db.transactionBegin();
    step7(&db);
    db.transactionCommit();
#endif

  }
  catch(eyedb::Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
