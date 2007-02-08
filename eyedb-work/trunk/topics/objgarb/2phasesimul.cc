
#include <assert.h>
#include <iostream>
#include <map>
#include <vector>

#define ELSE

static unsigned int ObjectCount = 0;
static unsigned int ObjectID = 1;

class Object;

static std::map<Object *, bool> obj_table;

class Object {

public:
  unsigned int id;
  const char *cls;
  std::map<Object *, bool> refby_map;
  bool cycle;

public:
  Object(const char *cls) : cls(cls) {
    id = ObjectID++;
    addRef(0);
    std::cout << "creating " << cls << " id #" << getID() << std::endl;
    ObjectCount++;
    obj_table[this] = true;
    cycle = false;
  }

  int getID() const {return id;}

  void addRef(Object *o) {
    refby_map[o] = true;
  }

  void unRef(Object *o) {
    assert(refBy(o));
    refby_map.erase(refby_map.find(o));
  }

  void reserve() {
  }

  void release() {
  }

  void garbage() {
    std::cout << "garbaging " << cls << " id #" << getID() << std::endl;
    obj_table.erase(obj_table.find(this));
    --ObjectCount;
  }

  bool refBy(Object *o) {
    return refby_map.find(o) != refby_map.end();
  }

  bool refBy_r(Object *o) {
    if (o == this)
      return true;

    if (cycle)
      return false;

    cycle = true;

    std::map<Object *, bool>::iterator begin = refby_map.begin();
    std::map<Object *, bool>::iterator end = refby_map.end();

    while (begin != end) {
      if ((*begin).first->refBy(o))
	return true;
      ++begin;
    }

    cycle = false;
    return false;
  }
};

void GC()
{
  std::map<Object *, bool>::iterator begin = obj_table.begin();
  std::map<Object *, bool>::iterator end = obj_table.end();

  std::vector<Object *> gc_v;

  while (begin != end) {
    if (!(*begin).first->refBy(0))
      gc_v.push_back((*begin).first);
    ++begin;
  }

  std::cout << "to garbage: " << gc_v.size() << std::endl;
  std::vector<Object *>::iterator b = gc_v.begin();
  std::vector<Object *>::iterator e = gc_v.end();
  while (b != e) {
    (*b)->garbage();
    ++b;
  }

  std::cout << "obj table: " << obj_table.size() << std::endl;
}

class B;
class C;

class A : public Object {

  B *b1, *b2;
  C *c1, *c2;

public:
  A() : Object("A"), b1(0), b2(0) { }

  B *get_b1() {return b1;}
  void set_b1(B *b1);

  B *get_b2() {return b2;}
  void set_b2(B *b2);

  C *get_c1() {return c1;}
  void set_c1(C *c1);

  C *get_c2() {return c2;}
  void set_c2(C *c2);
};

class B : public Object {

  A *a1, *a2;

public:
  B() : Object("B"), a1(0), a2(0) { }

  void set_a1(A *a);

  void set_a2(A *a);

  A *get_a1() {return a1;}
  A *get_a2() {return a2;}
};

class C : public Object {
  B *b1;
  B *b2;

public:
  C() : Object("C"), b1(0), b2(0) { }

  B *get_b1() {return b1;}
  void set_b1(B *b1);

  B *get_b2() {return b2;}
  void set_b2(B *b2);
};

void A::set_b1(B *b1)
{
  if (this->b1)
    this->b1->unRef(this);

  this->b1 = b1;

  if (this->b1) {
    this->b1->addRef(this);
  }
}

void A::set_b2(B *b2)
{
  if (this->b2)
    this->b2->unRef(this);

  this->b2 = b2;

  if (this->b2) {
    this->b2->addRef(this);
  }

}

void A::set_c1(C *c1)
{
  if (this->c1)
    this->c1->unRef(this);

  this->c1 = c1;

  if (this->c1) {
    this->c1->addRef(this);
  }
}

void A::set_c2(C *c2)
{
  if (this->c2)
    this->c2->unRef(this);

  this->c2 = c2;

  if (this->c2) {
    this->c2->addRef(this);
  }

}

void B::set_a1(A *a1)
{
  if (this->a1)
    this->a1->unRef(this);

  this->a1 = a1;

  if (this->a1) {
    this->a1->addRef(this);
  }
}

void B::set_a2(A *a2)
{
  if (this->a2)
    this->a2->unRef(this);

  this->a2 = a2;

  if (this->a2) {
    this->a2->addRef(this);
  }
}

void C::set_b1(B *b1)
{
  if (this->b1)
    this->b1->unRef(this);

  this->b1 = b1;

  if (this->b1) {
    this->b1->addRef(this);
  }
}

void C::set_b2(B *b2)
{
  if (this->b2)
    this->b2->unRef(this);

  this->b2 = b2;

  if (this->b2) {
    this->b2->addRef(this);
  }

}

int main(int argc, char *argv[])
{
  A *a = new A();

  B *b1 = new B();
  a->set_b1(b1);

  B *b2 = new B();
  a->set_b1(b2);

  b2->set_a1(a);

  B *b3 = new B();

  C *c1 = new C();
  C *c2 = new C();
  a->set_c1(c1);
  a->set_c2(c2);
  //a->set_c2(c1);

  c1->set_b2(b2);

  GC();

#if 1
  a->unRef(0);
  c1->unRef(0);
  c2->unRef(0);

  GC();
  b1->unRef(0);
  b2->unRef(0);
  b3->unRef(0);

#else
  a->unRef(0);
  b1->unRef(0);
  b2->unRef(0);
  GC();

  b3->unRef(0);

  c1->unRef(0);
  c2->unRef(0);
#endif

  GC();
  std::cout << "\nObjectCount: " << ObjectCount << std::endl;


  return 0;
}
