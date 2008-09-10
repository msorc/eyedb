#include <eyedb/eyedb.h>
#include "eyedbreflect_adapter.h"


vector<idbAttribute*> * getAttributeVector(idbClass *c){
  int cnt=0;
  idbAttribute **tattr=const_cast<idbAttribute**>(c->getAttributes(cnt));
  vector<idbAttribute*>*res= new vector<idbAttribute*>(cnt);
  for(int i=0;i<cnt;i++){
    (*res)[i]=tattr[i];
  }
  return res;

}

vector<idbClass*> * getClassVector(idbSchema *sch){
  const idbLinkedList *l = sch->getClassList();
  // foreach class in list, dump the class
  idbLinkedListCursor c(l);
  const idbClass *cls;
  vector<idbClass*> *res = new vector<idbClass*>();
  while (c.getNext((void *&)cls)){
    res->push_back(const_cast<idbClass*>(cls));
  }
  return res;

}


list<idbClass*> * getSchema_ListOfClass(idbSchema *sch){
  const idbLinkedList *l = sch->getClassList();
  // foreach class in list, dump the class
  idbLinkedListCursor c(l);
  const idbClass *cls;
  list<idbClass*> *res = new list<idbClass*>();
  while (c.getNext((void *&)cls)){
    res->push_back(const_cast<idbClass*>(cls));
  }
  return res;

}

/*
VectorAttribute * vecAttr(){
  return new VectorAttribute();
}
*/
ClassList::ClassList(const idbSchema *s){
  printf("classlistptr:%p\n",s);
  pimpl = s->getClassList();
}

ClassListCursor::ClassListCursor(ClassList *cl) : c(new idbLinkedListCursor(cl->pimpl)) {}

const idbClass *ClassListCursor::next(){
  return this->next_;
}
bool ClassListCursor::hasNext(){
  if(!c->getNext((void *&)this->next_)){
    return false;
  } 
  return true;
}
ClassListCursor::~ClassListCursor(){
  delete c;
}

AttributeList::AttributeList(const idbClass *cls){
  attrs=const_cast<idbAttribute**>(cls->getAttributes(attrcnt));
}

AttributeListCursor::AttributeListCursor(AttributeList *al) : crt(),alst(al){}
bool AttributeListCursor::hasNext(){
  return (++(this->crt) < alst->attrcnt);
}
const idbAttribute *AttributeListCursor::next(){
  //printf("trace : %p, %d, %s\n", alst->attrs[crt],crt, alst->attrs[crt]->getName());
  return alst->attrs[crt];
}

const char *readAttributeString(const idbObject *o, const char *name){
  const idbClass *cls = o->getClass();
  const idbAttribute *attr = cls->getAttribute(name);
  if(!attr) return 0;
  idbBool isnull;
  idbData s;
  attr->getValue(o, &s, idbAttribute::directAccess, 0, &isnull);
  if (isnull) return 0;
  return (const char*)s;

}

long readAttributeInteger(const idbObject *o, const char *name, int offset){
  const idbClass *cls = o->getClass();
  const idbAttribute *attr = cls->getAttribute(name);
  if(!attr) return 0;
  long  v;
  idbBool isnull;
  attr->getValue(o, (idbData *)&v, 1, offset, &isnull);
  if(isnull){
    v=0;
  }
  return v;
}



bool is_integer_attribute(const idbAttribute* a){
  const idbClass *c = a->getClass();
  return (c->asInt16Class()||c->asInt32Class()||
	  c->asInt64Class()||c->asCharClass()||
	  c->asEnumClass());

}


bool is_float_attribute(const idbAttribute* a){
  const idbClass *c = a->getClass();
  return c->asFloatClass();

}


bool is_string_attribute(const idbAttribute* a){
  return a->isString();
}

bool is_collection_attribute(const idbAttribute* a){
  const idbClass *c = a->getClass();
  return c->asCollectionClass();

}

bool is_object_attribute(const idbAttribute* a){
  return (a->isIndirect()||
	  a->getClass()->asAgregatClass());
}

const idbAttribute *get_attribute_def(const idbObject *o, const char *name){
  const idbClass *cls = o->getClass();
  return  cls->getAttribute(name);

}

long get_attribute_size(const idbObject *o, const idbAttribute* a){
  idbTypeModifier typmod = a->getTypeModifier();
  idbSize sz = 1;
  if (a->isVarDim()){
    a->getSize(o, sz);
  }
  long  rsz = typmod.pdims * sz;
  return rsz;
}
const idbObject* get_attribute_object_value(const idbObject *o, const idbAttribute* a, int offset){
  idbObject *oo=0;
  idbBool isnull;
  if(a->isIndirect()){
    idbOid oid;
    a->getOid(o, &oid, 1, offset);
   if(!oid.isValid()){
     return 0;
   }
   idbDatabase *db = const_cast<idbDatabase*>(a->getClass()->getSchema()->getDatabase());
   if(!db) return 0;
    db->loadObject(&oid,&oo);
    return oo;
  }
  a->getValue(o, (idbData *)&oo, 1, offset, &isnull);
  return oo;

}
static const char* void_str="";
const char *get_attribute_string_value(const idbObject *o, const idbAttribute* a){
  idbBool isnull;
  idbData s;
  a->getValue(o, &s, idbAttribute::directAccess, 0, &isnull);
  if (isnull) return void_str;
  return (const char*)s;

}

long get_attribute_integer_value(const idbObject *o, const idbAttribute* a, int offset){
  long  v;
  idbBool isnull;
  a->getValue(o, (idbData *)&v, 1, offset, &isnull);
  if(isnull){
    v=0;
  }
  return v;
}

double get_attribute_float_value(const idbObject *o, const idbAttribute* a, int offset){
  double v;
  idbBool isnull;
  a->getValue(o, (idbData *)&v, 1, offset, &isnull);
  if(isnull){
    v=0;
  }
  return v;
}

idbObjectArray *get_attribute_collection_value(const idbObject *o, const idbAttribute* a, int offset){
  idbObject *oo=0;
  a->getValue(o, (idbData *)&oo, 1, offset);
  if(!oo) return 0;
  idbCollection *coll = oo->asCollection();
  if(!coll) return 0;
  idbObjectArray *res = new  idbObjectArray();
  //coll->trace();
  coll->getElements(*res);
  return res;
}



