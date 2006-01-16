/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/


#include <eyedb/eyedb_p.h>
#include <sstream>

using namespace std;
//using std::ostringstream;

namespace eyedb {

int
Value::Struct::operator==(const Struct &stru)
{
  if (attr_cnt != stru.attr_cnt)
    return 0;

  for (int i = 0; i < attr_cnt; i++)
    {
      if (strcmp(attrs[i]->name, stru.attrs[i]->name))
	return 0;
      if (*attrs[i]->value != *stru.attrs[i]->value)
	return 0;
    }

  return 1;
}

void
Value::Struct::print(FILE *fd) const
{
  fprintf(fd, "struct(");
  for (int ii = 0; ii < attr_cnt; ii++)
    {
      if (ii) fprintf(fd, ", ");
      fprintf(fd, "%s: ", attrs[ii]->name);
      attrs[ii]->value->print(fd);
    }
  fprintf(fd, ")");
}

std::string
Value::Struct::toString() const
{
  std::string s = "struct(";
  for (int ii = 0; ii < attr_cnt; ii++)
    {
      if (ii) s += ", ";
      s += std::string(attrs[ii]->name) + ": " + attrs[ii]->value->getString();
    }
  return s + ")";
}

int Value::operator ==(const Value &val) const
{
  if (type != val.type)
    return 0;

  switch(type)
    {
    case NIL:
      return 1;

    case _NULL:
      return 1;

    case BOOL:
      return val.b == b;

    case BYTE:
      return val.by == by;

    case CHAR:
      return val.c == c;

    case SHORT:
      return val.s == s;

    case INT:
      return val.i == i;

    case LONG:
      return val.l == l;

    case DOUBLE:
      return val.d == d;

    case IDENT:
    case STRING:
      return !strcmp(val.str, str);

    case DATA:
      return val.data.data == data.data &&
	val.data.size == data.size;

    case OID:
      return *val.oid == *oid;

    case OBJECT:
      return val.o == o;

    case LIST:
    case BAG:
    case SET:
    case ARRAY:
      if (list->getCount() != val.list->getCount())
	return 0;
      return val.list == list;

    case POBJ:
      return val.idx == idx;

    case STRUCT:
      return *val.stru == *stru;

    default:
      abort();
    }

  return 0;
}

int Value::operator<(const Value &val) const
{
  if (type != val.type)
    return (int)type < (int)val.type;

  switch(type) {

  case NIL:
  case _NULL:
    return 0;

  case BOOL:
    return b < val.b;

  case BYTE:
    return by < val.by;

  case CHAR:
    return c < val.c;

  case SHORT:
    return s < val.s;

  case INT:
    return i < val.i;

  case LONG:
    return l < val.l;

  case DOUBLE:
    return d < val.d;

  case IDENT:
  case STRING:
    return strcmp(str, val.str) < 0 ? 1 : 0;

  case DATA: {
    Size size = data.size;
    if (val.data.size < size)
      size = val.data.size;
    return memcmp(data.data, val.data.data, size) < 0 ? 1 : 0;
  }

  case OID:
    return oid->getNX() < val.oid->getNX();

  case OBJECT: {
    Size o_size;
    Data o_idr = o->getIDR(o_size);

    Size val_o_size;
    Data val_o_idr = val.o->getIDR(val_o_size);

    Size size = (o_size < val_o_size ? o_size : val_o_size);
    return memcmp(o_idr, val_o_idr, size) < 0 ? 1 : 0;
  }

  default:
    return !(*this == val);
  }
}

int Value::operator !=(const Value &val) const
{
  return !(*this == val);
}

Value::Value(const Value &val)
{
  bufstr = NULL;
  type = NIL;
  *this = val;
}

Status
Value::toOidObjectArray(Database *db, LinkedList &ll, Bool isobj,
			   const RecMode *rcm)
{
  if (type == OID)
    {
      if (isobj)
	{
	  if (db)
	    {
	      Object *x;
	      Status status = db->loadObject(*oid, x, rcm);
	      if (status) return status;
	      ll.insertObjectLast(x);
	    }
	}
      else
	ll.insertObjectLast(new Oid(*oid));
    }
  else if (type == OBJECT)
    {
      if (isobj)
	ll.insertObjectLast(o);
      else if (o)
	{
	  Oid *xoid = new Oid(o->getOid());
	  ll.insertObjectLast(new Oid(*xoid));
	}
    }

  else if (type == LIST || type == BAG || type == SET || type == ARRAY)
    {
      LinkedListCursor cc(list);
      Value *v;
      Status status;
      while (cc.getNext((void *&)v))
	if (status = v->toOidObjectArray(db, ll, isobj, rcm))
	  return status;
    }
  else if (type == STRUCT)
    {
      Status status;
      for (int ii = 0; ii < stru->attr_cnt; ii++)
	if (status = stru->attrs[ii]->value->toOidObjectArray(db, ll, isobj, rcm))
	  return status;
    }

  return Success;
}

Status
Value::toValueArray(LinkedList &ll)
{
  if (type == LIST || type == BAG || type == SET || type == ARRAY)
    {
      LinkedListCursor cc(list);
      Value *v;
      Status status;
      while (cc.getNext((void *&)v))
	if (status = v->toValueArray(ll))
	  return status;
    }
  else if (type == STRUCT)
    {
      Status status;
      for (int ii = 0; ii < stru->attr_cnt; ii++)
	if (status = stru->attrs[ii]->value->toValueArray(ll))
	  return status;
    }
  else
    ll.insertObjectLast(new Value(*this));

  return Success;
}

Status
Value::toArray(Database *db, ObjectArray &objarr,
		  const RecMode *rcm)
{
  LinkedList ll;
  Status status = toOidObjectArray(db, ll, True, rcm);
  if (status) return status;

  objarr.set(0, ll.getCount());

  LinkedListCursor cc(ll);
  Object *x;
  for (int ii = 0; cc.getNext((void *&)x); ii++)
    objarr[ii] = x;

  return Success;
}

Status
Value::toArray(OidArray &oidarr)
{
  LinkedList ll;
  Status status = toOidObjectArray(0, ll, False, 0);
  if (status) return status;

  oidarr.set(0, ll.getCount());

  LinkedListCursor cc(ll);
  Oid *x;
  for (int ii = 0; cc.getNext((void *&)x); ii++)
    {
      oidarr[ii] = *x;
      delete x;
    }

  return Success;
}

Status
Value::toArray(ValueArray &valarr)
{
  LinkedList ll;
  Status status = toValueArray(ll);
  if (status) return status;

  valarr.set(0, ll.getCount());

  LinkedListCursor cc(ll);

  Value *x;
  for (int ii = 0; cc.getNext((void *&)x); ii++)
    {
      valarr[ii] = *x;
      delete x;
    }

  return Success;
}

Value &Value::operator =(const Value &val)
{
  if (this == &val)
    return *this;

  garbage();

  type = val.type;

  if (type == STRING || type == IDENT)
    str = strdup(val.str);
  else if (type == OID)
    oid = new Oid(*val.oid);
  else if (type == LIST || type == BAG || type == ARRAY || type == SET)
    {
      list = new LinkedList();
      LinkedListCursor cursor(val.list);
      Value *value;
      for (int ii = 0; cursor.getNext((void *&)value); ii++)
	if (value) list->insertObjectLast(new Value(*value));
    }
  else if (type == STRUCT)
    {
      stru = new Struct(val.stru->attr_cnt);
      for (int ii = 0; ii < stru->attr_cnt; ii++)
	stru->attrs[ii] = new Attr(val.stru->attrs[ii]->name,
				   val.stru->attrs[ii]->value);
    }
  else
    memcpy(this, &val, sizeof(*this));

  bufstr = NULL;

  return *this;
}

static void
append(char *& buf, const char *s)
{
  buf = (char *)realloc(buf, strlen(buf)+strlen(s)+1);
  strcat(buf, s);
}

//#define OPTIM_STRLIST

static std::string
getStringList(LinkedList *list, const char *head)
{
#ifdef OPTIM_STRLIST
  int len = 16;
  char *s = (char *)malloc(len);
  *s = 0;

  LinkedListCursor cursor(list);
  Value *value;
  for (int i = 0; cursor.getNext((void *&)value); i++)
    {
      if (isBackendInterrupted())
	{
	  set_backendInterrupt(False);
	  return "<interrupted>";
	}

      char *x = value->getString();
      int l = strlen(x);
      if (l + strlen(s) + 4 >= len)
	{
	  len += l + 200;
	  s = (char *)realloc(s, len);
	}

      if (i) strcat(s, ", ");
      strcat(s, x);
    }

  if (strlen(s) + 4 >= len)
    {
      len += 4;
      s = (char *)realloc(s, len);
    }

  strcat(s, ")");
  return std::string(s);
#else
  std::string s = std::string(head) + "(";

  LinkedListCursor cursor(list);
  Value *value;
  for (int i = 0; cursor.getNext((void *&)value); i++)
    {
      if (i) s += ", ";
      s += value->getString();
    }

  s += ")";
  return s;
#endif
}

static void
print_list(FILE *fd, LinkedList *list, const char *head)
{
  fprintf(fd, "%s(", head);

  LinkedListCursor cursor(list);
  Value *value;
  for (int i = 0; cursor.getNext((void *&)value); i++)
    {
      if (i) fprintf(fd, ", ");
      value->print(fd);
    }

  fprintf(fd, ")");
}

const char *Value::getString() const
{
  if (bufstr)
    return bufstr;

  char tok[32];

  *tok = 0;

  switch(type)
    {
    case NIL:
      ((Value *)this)->bufstr = strdup(NilString);
      break;

    case _NULL:
      ((Value *)this)->bufstr = strdup(NullString);
      break;

    case BOOL:
      sprintf(tok, "%s", (b ? "true" : "false"));
      break;

    case BYTE:
      sprintf(tok, "\\0%d", b);
      break;

    case CHAR:
      sprintf(tok, "'%c'", c);
      break;

    case SHORT:
      sprintf(tok, "%d", s);
      break;

    case INT:
      sprintf(tok, "%d", i);
      break;

    case LONG:
      sprintf(tok, "%lld", l);
      break;

    case DOUBLE:
      sprintf(tok, "%f", d);
      break;

    case IDENT:
      ((Value *)this)->bufstr = strdup(str);
      break;

    case STRING:
      ((Value *)this)->bufstr = (char *)malloc(strlen(str)+3);
      sprintf(((Value *)this)->bufstr, "\"%s\"", str);
      break;

    case DATA:
      sprintf(tok, "[0x%x, %u]", data.data, data.size);
      break;

    case OID:
      ((Value *)this)->bufstr = strdup(oid->getString());
      break;

    case OBJECT:
      {
	ostringstream ostr;
	ostr << o; // << ends;
	((Value *)this)->bufstr = strdup(ostr.str().c_str());
      }
      break;

    case LIST:
      ((Value *)this)->bufstr = strdup(getStringList(list, "list").c_str());
      break;

    case SET:
      ((Value *)this)->bufstr = strdup(getStringList(list, "set").c_str());
      break;

    case BAG:
      ((Value *)this)->bufstr = strdup(getStringList(list, "bag").c_str());
      break;

    case ARRAY:
      ((Value *)this)->bufstr = strdup(getStringList(list, "array").c_str());
      break;

    case POBJ:
      {
	std::string x = str_convert((long)idx, "%x:obj");
	((Value *)this)->bufstr = strdup(x.c_str());
      }
      break;

    case STRUCT:
      ((Value *)this)->bufstr = strdup(stru->toString().c_str());
      break;

    default:
      abort();
    }

  if (*tok)
    ((Value *)this)->bufstr = strdup(tok);

  return bufstr;
}

void
Value::print(FILE *fd) const
{
  switch(type)
    {
    case NIL:
      fprintf(fd, NilString);
      break;

    case _NULL:
      fprintf(fd, NullString);
      break;

    case BOOL:
      fprintf(fd, "%s", (b ? "true" : "false"));
      break;

    case BYTE:
      fprintf(fd, "\\0%d", b);
      break;

    case CHAR:
      fprintf(fd, "'%c'", c);
      break;

    case SHORT:
      fprintf(fd, "%d", s);
      break;

    case INT:
      fprintf(fd, "%d", i);
      break;

    case LONG:
      fprintf(fd, "%lld", l);
      break;

    case DOUBLE:
      fprintf(fd, "%f", d);
      break;

    case IDENT:
      fprintf(fd, "%s", str);
      break;

    case STRING:
      fprintf(fd, "\"%s\"", str);
      break;

    case DATA:
      fprintf(fd, "0x%x", data);
      break;

    case OID:
      fprintf(fd, oid->getString());
      break;

    case OBJECT:
      o->trace(fd);
      break;

    case LIST:
      print_list(fd, list, "list");
      break;

    case SET:
      print_list(fd, list, "set");
      break;

    case BAG:
      print_list(fd, list, "bag");
      break;

    case ARRAY:
      print_list(fd, list, "array");
      break;

    case POBJ:
      fprintf(fd, "%x:obj", idx);
      break;

    case STRUCT:
      fprintf(fd, "%s", stru->toString().c_str());
      break;

    default:
      abort();
    }
}

const char *
Value::getAttributeName(const Class *cl, Bool isIndirect)
{
  if (cl->asCharClass())
    {
      if (isIndirect)
	return "str";
      return "c";
    }

  if (isIndirect || (!cl->asBasicClass() && !cl->asEnumClass()))
    return "o";

  if (cl->asInt16Class())
    return "s";

  if (cl->asInt32Class())
    return "i";

  if (cl->asInt64Class())
    return "l";

  if (cl->asFloatClass())
    return "d";

  if (cl->asOidClass())
    return "oid";

  if (cl->asByteClass())
    return "by";

  return "<unknown class>";
}

ostream& operator<<(ostream& os, const Value &value)
{
  os << value.getString();
  return os;
}

ostream& operator<<(ostream& os, const Value *value)
{
  os << value->getString();
  return os;
}

void
Value::garbage()
{
  if (type == STRING || type == IDENT)
    free(str);
  else if (type == OID)
    delete oid;
  else if (type == LIST || type == BAG || type == SET || type == ARRAY)
    {
      LinkedListCursor cursor(list);
      Value *value;
      while (cursor.getNext((void *&)value))
	delete value;
      delete list;
    }
  else if (type == STRUCT)
    delete stru;

  free(bufstr);
}

const char *
Value::getStringType() const
{
  return getStringType(type);
}

const char *
Value::getStringType(Value::Type type)
{
  switch(type)
    {
    case NIL:
      return "nil";

    case _NULL:
      return "null";

    case BOOL:
      return "bool";

    case BYTE:
      return "byte";

    case CHAR:
      return "char";

    case SHORT:
      return "int16";

    case INT:
      return "int32";

    case LONG:
      return "int64";

    case DOUBLE:
      return "double";

    case IDENT:
      return "ident";

    case STRING:
      return "string";

    case DATA:
      return "data";

    case OID:
      return "oid";

    case OBJECT:
      return "object";

    case LIST:
      return "list";

    case SET:
      return "set";

    case BAG:
      return "bag";

    case ARRAY:
      return "array";

    case POBJ:
      return "pobject";

    case STRUCT:
      return "struct";

    default:
      return "<unknown>";
    }
}

Data
Value::getData(Size *psize) const
{
  switch(type)
    {
    case NIL:
    case _NULL:
      if (psize)
	*psize = 0;
      return 0;

    case BYTE:
      if (psize)
	*psize = sizeof(by);
      return (Data)&by;;

    case CHAR:
      if (psize)
	*psize = sizeof(c);
      return (Data)&c;;

    case SHORT:
      if (psize)
	*psize = sizeof(s);
      return (Data)&s;;

    case INT:
      if (psize)
	*psize = sizeof(i);
      return (Data)&i;

    case LONG:
      if (psize)
	*psize = sizeof(l);
      return (Data)&l;;

    case DOUBLE:
      if (psize)
	*psize = sizeof(d);
      return (Data)&d;;

    case STRING:
      if (psize)
	*psize = strlen(str)+1;
      return (Data)str;

    case DATA:
      if (psize)
	*psize = data.size;
      return data.data;

    case OID:
      if (psize)
	*psize = sizeof(oid);
      return (Data)&oid;;

    default:
      assert(0);
      if (psize)
	*psize = 0;
      return (Data)0;;
    }
}

void
Value::code(Data &idr, Offset &offset, Size &alloc_size) const
{
  char x = type;
  char_code(&idr, &offset, &alloc_size, &x);

  switch(type)
    {
    case NIL:
    case _NULL:
      break;

    case BOOL:
      x = b;
      char_code(&idr, &offset, &alloc_size, &x);
      break;

    case BYTE:
      char_code(&idr, &offset, &alloc_size, (char *)&by);
      break;

    case CHAR:
      char_code(&idr, &offset, &alloc_size, &c);
      break;

    case SHORT:
      int16_code(&idr, &offset, &alloc_size, &s);
      break;

    case INT:
      int32_code(&idr, &offset, &alloc_size, &i);
      break;

    case LONG:
      int64_code(&idr, &offset, &alloc_size, &l);
      break;

    case DOUBLE:
      double_code(&idr, &offset, &alloc_size, &d);
      break;

    case IDENT:
    case STRING:
      string_code(&idr, &offset, &alloc_size, str);
      break;

    case DATA:
      break;

    case OID:
      oid_code(&idr, &offset, &alloc_size, oid->getOid());
      break;

    case OBJECT:
      break;

    case LIST:
    case SET:
    case BAG:
    case ARRAY:
      {
	int cnt = list->getCount();
	int32_code(&idr, &offset, &alloc_size, &cnt);
	LinkedListCursor cc(list);
	Value *v;
	while (cc.getNext((void *&)v))
	  v->code(idr, offset, alloc_size);
      }
      break;

    case POBJ:
      int32_code(&idr, &offset, &alloc_size, (eyedblib::int32 *)&idx);
      break;

    case STRUCT:
      {
	int32_code(&idr, &offset, &alloc_size, &stru->attr_cnt);
	for (int ii = 0; ii < stru->attr_cnt; ii++)
	  {
	    string_code(&idr, &offset, &alloc_size, stru->attrs[ii]->name);
	    stru->attrs[ii]->value->code(idr, offset, alloc_size);
	  }

      }
      break;

    default:
      abort();
      break;
    }
}

Value::~Value()
{
  garbage();
}

ValueArray::ValueArray(const Collection *coll)
{
  values = NULL;
  value_cnt = 0;
  coll->getElements(*this);
}

ValueArray::ValueArray(const ValueArray &valarr)
{
  value_cnt = 0;
  values = NULL;
  *this = valarr;
}
 
ValueArray::ValueArray(const ValueList &list)
{
  value_cnt = 0;
  int cnt = list.getCount();
  if (!cnt)
    {
      values = 0;
      return;
    }

  values = (Value *)malloc(sizeof(Value) * cnt);
  memset(values, 0, sizeof(Value) * cnt);

  ValueListCursor c(list);
  Value value;

  for (; c.getNext(value); value_cnt++)
    values[value_cnt] = value;
}

ValueArray &ValueArray::operator=(const ValueArray &valarr)
{
  set(valarr.values, valarr.value_cnt, True);
  return *this;
}

void ValueArray::set(Value *_values, int _value_cnt, Bool copy)
{
  if (values)
    delete[] values;

  value_cnt = _value_cnt;

  if (copy)
    {
      values = new Value[value_cnt];
      
      if (_values)
	{
	  for (int i = 0; i < value_cnt; i++)
	    values[i] = _values[i];
	}

      return;
    }

  values = _values;
}

ValueList *
ValueArray::toList() const
{
  return new ValueList(*this);
}

ValueArray::~ValueArray()
{
  if (values)
    delete [] values;
}

void
Value::decode(Data idr, Offset &offset)
{
  garbage();

  char x;
  char_decode(idr, &offset, &x);
  type = (Type)x;

  switch(type)
    {
    case NIL:
    case _NULL:
      break;

    case BOOL:
      char_decode(idr, &offset, &x);
      b = (Bool)x;
      break;

    case BYTE:
      char_decode(idr, &offset, (char *)&by);
      break;

    case CHAR:
      char_decode(idr, &offset, &c);
      break;

    case SHORT:
      int16_decode(idr, &offset, &s);
      break;

    case INT:
      int32_decode(idr, &offset, &i);
      break;

    case LONG:
      int64_decode(idr, &offset, &l);
      break;

    case DOUBLE:
      double_decode(idr, &offset, &d);
      break;

    case IDENT:
    case STRING:
      {
	char *y;
	string_decode(idr, &offset, &y);
	str = strdup(y);
      }
      break;

    case DATA:
      break;

    case OID:
      {
	eyedbsm::Oid xoid;
	oid_decode(idr, &offset, &xoid);
	oid = new Oid(xoid);
      }
      break;

    case OBJECT:
      break;

    case LIST:
    case SET:
    case BAG:
    case ARRAY:
      {
	int cnt;
	int32_decode(idr, &offset, &cnt);
	list = new LinkedList();
	for (int ii = 0; ii < cnt; ii++)
	  {
	    Value *v = new Value();
	    v->decode(idr, offset);
	    list->insertObjectLast(v);
	  }
      }
      break;

    case POBJ:
      int32_decode(idr, &offset, (eyedblib::int32 *)&idx);
      break;

    case STRUCT:
      {
	int cnt;
	int32_decode(idr, &offset, &cnt);
	stru = new Struct(cnt);
	for (int ii = 0; ii < stru->attr_cnt; ii++)
	  {
	    char *y;
	    string_decode(idr, &offset, &y);
	    Value *v = new Value();
	    v->decode(idr, offset);
	    stru->attrs[ii] = new Attr(y, v);
	  }

      }
      break;

    default:
      abort();
      break;
    }
}

void decode_value(void *xdata, void *xvalue)
{
  Offset offset = 0;
  ((Value *)xvalue)->decode((Data)((rpc_Data *)xdata)->data, offset);
}

ValueList::ValueList()
{
  list = new LinkedList();
}

ValueList::ValueList(const ValueArray &value_array)
{
  list = new LinkedList();
  for (int i = 0; i < value_array.getCount(); i++)
    insertValueLast(value_array[i]);
}

int ValueList::getCount() const
{
  return list->getCount();
}

void
ValueList::insertValue(const Value &value)
{
  list->insertObject(new Value(value));
}

void
ValueList::insertValueFirst(const Value &value)
{
  list->insertObjectFirst(new Value(value));
}

void
ValueList::insertValueLast(const Value &value)
{
  list->insertObjectLast(new Value(value));
}

Bool
ValueList::suppressValue(const Value &value)
{
  LinkedListCursor c(list);
  Value *xvalue;
  while (c.getNext((void *&)xvalue))
    if (*xvalue == value)
      {
	list->deleteObject(xvalue);
	return True;
      }

  return False;
}

Bool
ValueList::exists(const Value &value) const
{
  LinkedListCursor c(list);
  Value *xvalue;
  while (c.getNext((void *&)xvalue))
    if (*xvalue == value)
      return True;
  return False;
}

void
ValueList::empty()
{
  list->empty();
}

ValueArray *
ValueList::toArray() const
{
  int cnt = list->getCount();
  if (!cnt)
    return new ValueArray();
  Value *arr = (Value *)malloc(sizeof(Value) * cnt);
  memset(arr, 0, sizeof(Value) * cnt);

  LinkedListCursor c(list);
  Value *xvalue;
  for (int i = 0; c.getNext((void *&)xvalue); i++)
    arr[i] = *xvalue;
  
  ValueArray *value_arr = new ValueArray(arr, cnt);
  free(arr);
  return value_arr;
}

ValueList::~ValueList()
{
  LinkedListCursor c(list);
  Value *xvalue;
  while (c.getNext((void *&)xvalue))
    delete xvalue;
  delete list;
}

ValueListCursor::ValueListCursor(const ValueList &valuelist)
{
  c = new LinkedListCursor(valuelist.list);
}

ValueListCursor::ValueListCursor(const ValueList *valuelist)
{
  c = new LinkedListCursor(valuelist->list);
}

Bool
ValueListCursor::getNext(Value &value)
{
  Value *xvalue;
  if (c->getNext((void *&)xvalue))
    {
      value = *xvalue;
      return True;
    }

  return False;
}

ValueListCursor::~ValueListCursor()
{
  delete c;
}
}
