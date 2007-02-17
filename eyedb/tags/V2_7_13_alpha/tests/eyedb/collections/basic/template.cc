
#include "schema.h"

class TCollection {

public:
  Status realize(const RecMode *rcm = RecMode::NoRecurs) {
    return getColl()->realize(rcm);
  }

  Status remove(const RecMode *rcm = RecMode::NoRecurs) {
    return getColl()->remove(rcm);
  }

  void setImplementation(const IndexImpl *idximpl) {
    return getColl()->setImplementation(idximpl);
  }

  Status getImplementation(IndexImpl *&idximpl, Bool remote = False) const {
    return getCollC()->getImplementation(idximpl, remote);
  }

  /*
  Status getImplStats(std::string &xstats, Bool dspImpl = True,
		      Bool full = False, const char *indent = "");

  Status getImplStats(IndexStats *&stats);

  Status simulate(const IndexImpl &idximpl, std::string &xstats,
		  Bool dspImpl = True, Bool full = False,
		  const char *indent = "");

  Status simulate(const IndexImpl &idximpl, IndexStats *&stats);
  */
  
  virtual Collection *getColl() = 0;
  virtual const Collection *getCollC() const = 0;

protected:
};

template <class T>
class TCollSet : public TCollection {

public:

  TCollSet(Database *db, const char *name = "", const IndexImpl *idximpl = 0) {
    coll = new CollSet(db, name, db->getSchema()->getClass("Person"),
		       True, idximpl);
  }

  TCollSet(CollSet *coll) : coll(coll) {}
  Status insert(const T item_o, Bool noDup = False) {
    return coll->insert(item_o, noDup);
  }

  Status getElements(vector<T> v) const {
    ObjectArray obj_arr;
    Status s = getCollC()->getElements(obj_arr);
    if (s)
      return s;
    v.erase(v.begin(), v.end());
    // push back every obj_arr
    return Success;
  }


  Status insert(const Oid &item_oid, Bool noDup = False) {
    return coll->insert(item_oid, noDup);
  }

  Collection *getColl() {return coll;}
  const Collection *getCollC() const {return coll;}

private:
  CollSet *coll;
};

template <typename T>
class TCollSet_L : public TCollection {

public:
  TCollSet_L(CollSet *coll) : coll(coll) {}

  Status insert(const T *item_o, Bool noDup = False) {
    return coll->insert(item_o, noDup);
  }

  Status realize(const RecMode *rcm = RecMode::NoRecurs) {
    return coll->realize(rcm);
  }

  Status remove(const RecMode *rcm = RecMode::NoRecurs) {
    return coll->remove(rcm);
  }

  Collection *getColl() {return coll;}
  const Collection *getCollC() const {return coll;}
  CollSet *getCollSet() {return coll;}

private:
  CollSet *coll;
};

template <>
class TCollSet_L<eyedblib::int32> : public TCollection {

public:
  TCollSet_L(CollSet *coll) : coll(coll) {}

  Status insert(eyedblib::int32 val, Bool noDup = False) {
    return coll->insert((Data)&val, noDup);
  }

  Collection *getColl() {return coll;}
  const Collection *getCollC() const {return coll;}

private:
  CollSet *coll;
};


/*
template <class U, bool>
class Z {
public:
  void perform(U *);
};

template <class U>
class ZZ {
public:
  void perform(U *);
};
*/

main()
{
  Person pp;
  typeid(Person).name();
  TCollSet_L<Person> c_p(0);
  TCollSet<Person*> c_p_ref((CollSet *)0);
  TCollSet_L<eyedblib::int32> c_int(0);

  Person *p = new Person();
  c_p.insert(p);
  c_p_ref.insert(p);

  /*
  Z<Person *, true> zz0;
  Z<Person *, 2> zz1;
  */

  /*
  const Person ppp;
  const Person *pp = 0;
  Z<pp> z;
  */
  return 0;
}
