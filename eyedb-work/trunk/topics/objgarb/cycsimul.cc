
#include <assert.h>
#include <iostream>
#include <map>

#define ELSE

static unsigned int ObjectCount = 0;
static unsigned int ObjectID = 1;

class Object {
  int refcnt;
  std::map<Object *, unsigned int> cycmap;
public:
  unsigned int id;
  const char *cls;
protected:
  bool cycle;

public:
  Object(const char *cls) : cls(cls), refcnt(1), cycle(false) {
    id = ObjectID++;
    std::cout << "creating " << cls << " id #" << getID() << std::endl;
    ObjectCount++;
  }

  int incr() {return ++refcnt;}

  int decr() {

    std::cout << cls << " #" << id << " cycmap.size " << cycmap.size() << std::endl;
    if (cycmap.size() != 0) {

      std::map<Object *, unsigned int>::iterator begin = cycmap.begin(); 
      std::map<Object *, unsigned int>::iterator end = cycmap.end(); 

      unsigned int sum = 0;
      while (begin != end) {
	sum += (*begin).second;
	++begin;
      }

      cycmap.clear();
      std::cout << "sum: " << sum << std::endl;
      refcnt -= sum;
    }

    --refcnt;

    //assert(refcnt >= 0);

    return refcnt;
  }

  int getRefcount() const {return refcnt;}
  int getID() const {return id;}

  void garbage(const char *cls) {
    std::cout << "garbaging " << cls << " id #" << getID() << std::endl;
    --ObjectCount;
  }

  void setCycle(Object *cycle_o) {
    if (cycmap.find(cycle_o) == cycmap.end())
      cycmap[cycle_o] = 1;
    else
      cycmap[cycle_o]++;
  }

  virtual void release() { }
  virtual bool detectCycle(Object *o) { }

  bool detectCycle_r(Object *ref) {
    if (ref)
      printf("looking for cycle %s #%d %s #%d\n", cls, id, ref->cls, ref->id);
    if (ref)
      return ref->detectCycle(this);
    return false;
  }
};

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

  bool detectCycle(Object *o);
  void release();
};

class B : public Object {

  A *a1, *a2;

public:
  B() : Object("B"), a1(0), a2(0) { }

  void set_a1(A *a);

  void set_a2(A *a);

  A *get_a1() {return a1;}
  A *get_a2() {return a2;}

  bool detectCycle(Object *o);
  void release();
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

  bool detectCycle(Object *o);
  void release();
};

#define SET_MTH(BASE_CLS, CLS_REF, ATTR) \
void BASE_CLS::set_##ATTR(CLS_REF *ATTR) \
{ \
  if (this->ATTR) \
    this->ATTR->release(); \
\
  this->ATTR = ATTR; \
  this->ATTR->incr(); \
}

void A::set_b1(B *b1)
{
  if (this->b1)
    this->b1->release();

  this->b1 = b1;

  if (this->b1) {
    if (detectCycle_r(this->b1->get_a1()))
      this->b1->setCycle(this);
    ELSE if (detectCycle_r(this->b1->get_a2()))
      this->b1->setCycle(this);

    this->b1->incr();
  }
}

void A::set_b2(B *b2)
{
  if (this->b2)
    this->b2->release();

  this->b2 = b2;

  if (this->b2) {
    if (detectCycle_r(this->b2->get_a1()))
      this->b2->setCycle(this);
    ELSE if (detectCycle_r(this->b2->get_a2()))
      this->b2->setCycle(this);

    this->b2->incr();
  }

}

void A::set_c1(C *c1)
{
  if (this->c1)
    this->c1->release();

  this->c1 = c1;

  if (this->c1) {
    if (detectCycle_r(this->c1->get_b1()))
      this->c1->setCycle(this);
    ELSE if (detectCycle_r(this->c1->get_b2()))
      this->c1->setCycle(this);

    this->c1->incr();
  }
}

void A::set_c2(C *c2)
{
  if (this->c2)
    this->c2->release();

  this->c2 = c2;

  if (this->c2) {
    if (detectCycle_r(this->c2->get_b1()))
      this->c2->setCycle(this);
    ELSE if (detectCycle_r(this->c2->get_b2()))
      this->c2->setCycle(this);

    this->c2->incr();
  }

}

void B::set_a1(A *a1)
{
  if (this->a1)
    this->a1->release();

  this->a1 = a1;

  if (this->a1) {
    if (detectCycle_r(this->a1->get_b1()))
      this->a1->setCycle(this);
    ELSE if (detectCycle_r(this->a1->get_b2()))
      this->a1->setCycle(this);
    ELSE if (detectCycle_r(this->a1->get_c1()))
      this->a1->setCycle(this);
    ELSE if (detectCycle_r(this->a1->get_c2()))
      this->a1->setCycle(this);

    this->a1->incr();
  }
}

void B::set_a2(A *a2)
{
  if (this->a2)
    this->a2->release();

  this->a2 = a2;

  if (this->a2) {
    if (detectCycle_r(this->a2->get_b1()))
      this->a2->setCycle(this);
    ELSE if (detectCycle_r(this->a2->get_b2()))
      this->a2->setCycle(this);
    ELSE if (detectCycle_r(this->a2->get_c1()))
      this->a2->setCycle(this);
    ELSE if (detectCycle_r(this->a2->get_c2()))
      this->a2->setCycle(this);

    this->a2->incr();
  }
}

void C::set_b1(B *b1)
{
  if (this->b1)
    this->b1->release();

  this->b1 = b1;

  if (this->b1) {
    if (detectCycle_r(this->b1->get_a1()))
      this->b1->setCycle(this);
    ELSE if (detectCycle_r(this->b1->get_a2()))
      this->b1->setCycle(this);

    this->b1->incr();
  }
}

void C::set_b2(B *b2)
{
  if (this->b2)
    this->b2->release();

  this->b2 = b2;

  if (this->b2) {
    if (detectCycle_r(this->b2->get_a1()))
      this->b2->setCycle(this);
    ELSE if (detectCycle_r(this->b2->get_a2()))
      this->b2->setCycle(this);

    this->b2->incr();
  }

}

void A::release()
{
  if (!decr()) {
    if (b1)
      b1->release();

    if (b2)
      b2->release();

    garbage("A");
  }
}

bool A::detectCycle(Object *o)
{
  if (cycle)
    return false;

  cycle = true;

  if (o == this) {
    printf("detect %s #%d cycle\n", cls, id);
    cycle = false;
    return true;
  }

  if (b1 && b1->detectCycle(o)) {
    cycle = false;
    return true;
  }

  if (b2 && b2->detectCycle(o)) {
    cycle = false;
    return true;
  }

  if (c1 && c1->detectCycle(o)) {
    cycle = false;
    return true;
  }

  if (c2 && c2->detectCycle(o)) {
    cycle = false;
    return true;
  }

  cycle = false;

  return false;
}

void B::release()
{
  if (!decr()) {
    if (a1)
      a1->release();

    if (a2)
      a2->release();

    garbage("B");
  }
}

bool B::detectCycle(Object *o)
{
  if (cycle)
    return false;

  cycle = true;

  if (o == this) {
    printf("detect %s #%d cycle\n", cls, id);
    cycle = false;
    return true;
  }

  if (a1 && a1->detectCycle(o)) {
    cycle = false;
    return true;
  }

  if (a2 && a2->detectCycle(o)) {
    cycle = false;
    return true;
  }

  cycle = false;
  return false;
}

void C::release()
{
  if (!decr()) {
    if (b1)
      b1->release();

    if (b2)
      b2->release();

    garbage("A");
  }
}

bool C::detectCycle(Object *o)
{
  if (cycle)
    return false;

  cycle = true;

  if (o == this) {
    printf("detect %s #%d cycle\n", cls, id);
    cycle = false;
    return true;
  }

  if (b1 && b1->detectCycle(o)) {
    cycle = false;
    return true;
  }

  if (b2 && b2->detectCycle(o)) {
    cycle = false;
    return true;
  }

  cycle = false;
  return false;
}

int main(int argc, char *argv[])
{
  A *a = new A();

  B *b1 = new B();
  a->set_b1(b1);

  B *b2 = new B();
  a->set_b1(b2);

  b2->set_a1(a);
  //  b2->set_a2(a);

  B *b3 = new B();
  /*
  a->set_b1(b3);
  */

#define USE_C
#ifdef USE_C
  C *c1 = new C();
  C *c2 = new C();
  a->set_c1(c1);
  a->set_c2(c2);

  /*
  printf("here 1\n");
  c1->set_b1(b1);
  */
  c1->set_b2(b2);
#endif

  a->release();
  b1->release();
  b2->release();
  b3->release();

#ifdef USE_C
  c1->release();
  c2->release();
#endif

  std::cout << "\nObjectCount: " << ObjectCount << std::endl;


  return 0;
}
