//#include <eyedb/eyedb.h>
#ifndef eyedbreflect_adapter_h
#define eyedbreflect_adapter_h
#include <vector>
#include <list>
namespace eyedb {
class Schema;
class Class;
class LinkedList;
class LinkedListCursor;
class Attribute;
};
//bool is_void_obj(void *p);

using namespace std;
/*
typedef vector<idbAttribute*> VectorAttribute;

VectorAttribute * vecAttr();
*/

typedef eyedb::Attribute idbAttribute;
typedef eyedb::Class idbClass;
typedef eyedb::Schema idbSchema;
typedef eyedb::LinkedList idbLinkedList;
typedef eyedb::LinkedListCursor idbLinkedListCursor;

vector<idbAttribute*> * getAttributeVector(idbClass *c);
vector<idbClass*> * getClassVector(idbSchema *c);
//unused
list<idbClass*> * getSchema_ListOfClass(idbSchema *c);


class ClassList{
 public:
  //ClassList();
  ClassList(const idbSchema *s);
  const idbLinkedList *pimpl;
};

class ClassListCursor{
 public:
  ~ClassListCursor();
  ClassListCursor(ClassList *cl) ;
  bool hasNext();
  const idbClass *next();
  idbClass *next_;
  idbLinkedListCursor *c;
};


class AttributeList{
 public:
  AttributeList(const idbClass *cls);
 private:
  idbAttribute **attrs;
  int attrcnt;
  friend class AttributeListCursor;
};

class AttributeListCursor{
 public:
  ~AttributeListCursor(){}
  AttributeListCursor(AttributeList *al);
  bool hasNext();
  const idbAttribute *next();
  private:
  int crt;
  AttributeList *alst;
};
class idbObject;
class idbObjectArray;

const char *readAttributeString(const idbObject *o, const char *name);
long readAttributeInteger(const idbObject *o, const char *name, int offset=0);

bool is_integer_attribute(const idbAttribute* a);
bool is_string_attribute(const idbAttribute* a);
bool is_float_attribute(const idbAttribute* a);

bool is_collection_attribute(const idbAttribute* a);
bool is_object_attribute(const idbAttribute* a);

const idbAttribute *get_attribute_def(const idbObject *o, const char *name);
long get_attribute_size(const idbObject *o, const idbAttribute* a);

const idbObject* get_attribute_object_value(const idbObject *o, const idbAttribute* a, int offset=0);
const char *get_attribute_string_value(const idbObject *o, const idbAttribute* a);
long get_attribute_integer_value(const idbObject *o, const idbAttribute* a, int offset=0);
double get_attribute_float_value(const idbObject *o, const idbAttribute* a, int offset=0);
idbObjectArray *get_attribute_collection_value(const idbObject *o, const idbAttribute* a, int offset=0);




#endif
