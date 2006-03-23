/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
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


#ifndef _EYEDB_VALUE_H
#define _EYEDB_VALUE_H

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  class ValueArray;
  class ObjectArray;

  /**
     Not yet documented.
  */
  class Value {

    // ----------------------------------------------------------------------
    // Value Interface
    // ----------------------------------------------------------------------

  public:

    enum Type {
      tNil = 0,
      tNull,
      tBool,
      tByte,
      tChar,
      tShort,
      tInt,
      tLong,
      tDouble,
      tIdent,
      tString,
      tData,
      tOid,
      tObject,
      tPobj,
      tList,
      tSet,
      tArray,
      tBag,
      tStruct
    } type;

    struct Attr {
      char *name;
      Value *value;

      Attr() {
	name = 0;
	value = 0;
      }

      Attr(const char *_name, Value *_value) {
	name = strdup(_name);
	value = _value;
      }

      ~Attr() {
	free(name);
      }
    };

    struct Struct {
      int attr_cnt;
      Value::Attr **attrs;

      Struct() {attr_cnt = 0; attrs = 0;}

      Struct(int _attr_cnt) {
	attr_cnt = _attr_cnt;
	attrs = new Attr*[attr_cnt];
      }

      void print(FILE *fd) const;
      int operator==(const Struct &);
      std::string toString() const;

      ~Struct() {
	for (int ii = 0; ii < attr_cnt; ii++)
	  delete attrs[ii];

	if (attr_cnt)
	  delete [] attrs;
      }

    private:
      Struct(const Struct &);
      Struct& operator=(const Struct &);
    };

    union {
      Bool b;
      unsigned char by;
      char c;
      short s;
      eyedblib::int32 i;
      eyedblib::int64 l;
      double d;
      char *str;
      struct {
	Data data;
	Size size;
      } data;
      Oid *oid;
      Object *o;
      unsigned int idx;
      LinkedList *list;
      Value::Struct *stru;
    };

    /**
       Not yet documented
    */
    Value() {
      bufstr = NULL;
      set();
    }

    /**
       Not yet documented
       @param b1
       @param b2
    */
    Value(Bool b1, Bool b2) {
      bufstr = NULL;
      set(b1, b2);
    }

    /**
       Not yet documented
       @param _b
    */
    Value(Bool _b) {
      bufstr = NULL;
      set(_b);
    }

    /**
       Not yet documented
       @param _by
    */
    Value(unsigned char _by) {
      bufstr = NULL;
      set(_by);
    }

    /**
       Not yet documented
       @param _c
    */
    Value(char _c) {
      bufstr = NULL;
      set(_c);
    }

    /**
       Not yet documented
       @param _s
    */
    Value(short _s) {
      bufstr = NULL;
      set(_s);
    }

    /**
       Not yet documented
       @param _d
    */
    Value(double _d) {
      bufstr = NULL;
      set(_d);
    }

    /**
       Not yet documented
       @param _i
    */
    Value(eyedblib::int32 _i) {
      bufstr = NULL;
      set(_i);
    }

    /**
       Not yet documented
       @param _l
    */
    Value(eyedblib::int64 _l) {
      bufstr = NULL;
      set(_l);
    }

    /**
       Not yet documented
       @param _str
    */
    Value(const char *_str) {
      bufstr = NULL;
      set(_str);
    }

    /**
       Not yet documented
       @param _str
       @param isident
    */
    Value(const char *_str, Bool isident) {
      bufstr = NULL;
      set(_str, isident);
    }

    /**
       Not yet documented
       @param _data
    */
    Value(Data _data, Size _size) {
      bufstr = NULL;
      set(_data, _size);
    }

    /**
       Not yet documented
       @param _oid
    */
    Value(const Oid &_oid) {
      bufstr = NULL;
      set(_oid);
    }

    /**
       Not yet documented
       @param _o
    */
    Value(const Object *_o) {
      bufstr = NULL;
      set(_o);
    }

    /**
       Not yet documented
       @param _o
       @param _idx
    */
    Value(const Object *_o, unsigned int _idx) {
      bufstr = NULL;
      set(_o, _idx);
    }

    /**
       Not yet documented
       @param _o
    */
    Value(Object *_o) {
      bufstr = NULL;
      set(_o);
    }

    /**
       Not yet documented
       @param _list
       @param _type
    */
    Value(LinkedList *_list, Value::Type _type) {
      bufstr = NULL;
      set(_list, _type);
    }

    /**
       Not yet documented
       @param _stru
    */
    Value(Value::Struct *_stru) {
      bufstr = NULL;
      set(_stru);
    }

    /**
       Not yet documented
    */
    void set() {
      type = tNil;
      unvalid();
    }

    /**
       Not yet documented
       @param b1
       @param b2
    */
    void set(Bool b1, Bool b2) {
      type = tNull;
      unvalid();
    }

    /**
       Not yet documented
       @param _b
    */
    void set(Bool _b) {
      type = tBool;
      b = _b;
      unvalid();
    }

    /**
       Not yet documented
       @param _by
    */
    void set (unsigned char _by) {
      type = tByte;
      by = _by;
      unvalid();
    }

    /**
       Not yet documented
       @param _c
    */
    void set(char _c) {
      type = tChar;
      c = _c;
      unvalid();
    }

    /**
       Not yet documented
       @param _s
    */
    void set(short _s) {
      type = tShort;
      s = _s;
      unvalid();
    }

    /**
       Not yet documented
       @param _d
    */
    void set(double _d) {
      type = tDouble;
      d = _d;
      unvalid();
    }

    /**
       Not yet documented
       @param _i
    */
    void set(eyedblib::int32 _i) {
      type = tInt;
      i = _i;
      unvalid();
    }

    /**
       Not yet documented
       @param _l
    */
    void set(eyedblib::int64 _l) {
      type = tLong;
      l = _l;
      unvalid();
    }

    /**
       Not yet documented
       @param *_str
    */
    void set(const char *_str) {
      type = tString;
      str = _str ? strdup(_str) : NULL;
      unvalid();
    }

    /**
       Not yet documented
       @param _str
       @param isident
    */
    void set(const char *_str, Bool isident) {
      type = (isident ? tIdent : tString);
      str = _str ? strdup(_str) : NULL;
      unvalid();
    }

    /**
       Not yet documented
       @param _data
    */
    void set(Data _data, Size _size) {
      type = tData;
      data.data = _data;
      data.size = _size;
      unvalid();
    }

    /**
       Not yet documented
       @param &_oid
    */
    void set(const Oid &_oid) {
      type = tOid;
      oid = new Oid(_oid);
      unvalid();
    }

    /**
       Not yet documented
       @param *_o
    */
    void set(const Object *_o) {
      type = tObject;
      o = (Object *)_o;
      unvalid();
    }

    /**
       Not yet documented
       @param _o
       @param _idx
    */
    void set(const Object *_o, unsigned int _idx) {
      type = tPobj;
      idx = _idx;
      unvalid();
    }

    /**
       Not yet documented
       @param *_o
    */
    void set(Object *_o) {
      type = tObject;
      o = _o;
      unvalid();
    }

    /**
       Not yet documented
       @param _list
       @param _type
    */
    void set(LinkedList *_list, Value::Type _type) {
      if (_type != tList && _type != tSet && _type != tArray && _type != tBag)
	{
	  (void)Exception::make("setting collection value: type must be "
				"tList, tSet, tArray or tBag");
	  return;
	}

      type = _type;
      list = _list;
      unvalid();
    }

    /**
       Not yet documented
       @param *_stru
    */
    void set(Value::Struct *_stru) {
      type = tStruct;
      stru = _stru;
      unvalid();
    }

    // flatten methods
    /**
       Not yet documented
       @param db
       @param objarr
       @param recmode
       @return
    */
    Status toArray(Database *db, ObjectArray &objarr,
		   const RecMode *recmode = RecMode::NoRecurs);

    /**
       Not yet documented
       @param valarr
       @return
    */
    Status toArray(ValueArray &valarr);

    /**
       Not yet documented
       @param oidarr
       @return
    */
    Status toArray(OidArray &oidarr);

    /**
       Not yet documented
       @param val
    */
    Value(const Value &val);

    /**
       Not yet documented
       @param val
       @return
    */
    int operator ==(const Value &val) const;

    /**
       Not yet documented
       @param val
       @return
    */
    int operator <(const Value &val) const;

    /**
       Not yet documented
       @param val
       @return
    */
    int operator !=(const Value &val) const;

    /**
       Not yet documented
       @param val
       @return
    */
    Value &operator =(const Value &val);

    /**
       Not yet documented
       @param fd
    */
    void trace(FILE *fd = stdout) const {
      print(fd);
    }

    /**
       Not yet documented
       @param fd
    */
    void print(FILE *fd = stdout) const;

    /**
       Not yet documented
       @return
    */
    Data getData(Size *psize = 0) const;

    /**
       Not yet documented
       @return
    */
    const char *getString() const;

    /**
       Not yet documented
       @return
    */
    const char *getStringType() const;

    /**
       Not yet documented
       @return
    */
    Value::Type getType() const {return type;}

    /**
       Not yet documented
       @param type
       @return
    */
    static const char *getStringType(Value::Type type);

    /**
       Not yet documented
       @param cl
       @param isIndirect
       @return
    */
    static const char *getAttributeName(const Class *cl, Bool isIndirect);

    /**
       Not yet documented
    */
    void init() {
      bufstr = NULL;
      type = tNil;
    }

    /**
       Not yet documented
       @param idr
       @param offset
    */
    void decode(Data idr, Offset &offset);

    /**
       Not yet documented
       @param idr
       @param offset
       @param alloc_size
    */
    void code(Data &idr, Offset &offset, Size &alloc_size) const;

    ~Value();

  private:
    Status toValueArray(LinkedList &);
    Status toOidObjectArray(Database *db, LinkedList &,
			    Bool isObj, const RecMode *rcm);
    void garbage();
    char *bufstr;
    void unvalid() {
      free(bufstr);
      bufstr = NULL;
    }
  };

  std::ostream& operator<<(std::ostream&, const Value &);
  std::ostream& operator<<(std::ostream&, const Value *);

  class ValueList;

  class ValueArray {

    // ----------------------------------------------------------------------
    // ValueArray Interface
    // ----------------------------------------------------------------------

  public:
    Value *values;
    int value_cnt;

    /**
       Not yet documented
    */
    ValueArray() {
      values = NULL;
      value_cnt = 0;
    }

    /**
       Not yet documented
       @param _values
       @param _value_cnt
       @param copy
    */
    ValueArray(Value *_values, int _value_cnt, Bool copy = True) {
      values = NULL;
      value_cnt = 0;
      set(_values, _value_cnt, copy);
    }

    /**
       Not yet documented
       @param valarr
    */
    ValueArray(const ValueArray &valarr);

    /**
       Not yet documented
       @param coll
    */
    ValueArray(const Collection *coll);

    /**
       Not yet documented
       @param list
    */
    ValueArray(const ValueList &list);

    /**
       Not yet documented
       @param valarr
       @return
    */
    ValueArray &operator=(const ValueArray &valarr);

    /**
       Not yet documented
       @return
    */
    int getCount() const {return value_cnt;}

    /**
       Not yet documented
       @param _values
       @param _value_cnt
       @param copy
    */
    void set(Value *_values, int _value_cnt, Bool copy = True);

    /**
       Not yet documented
       @param x
       @return
    */
    Value & operator[](int x) {
      return values[x];
    }

    /**
       Not yet documented
       @param x
       @return
    */
    const Value & operator[](int x) const {
      return values[x];
    }

    /**
       Not yet documented
       @return
    */
    ValueList *toList() const;

    ~ValueArray();
  };

  class ValueListCursor;

  /**
     Not yet documented.
  */
  class ValueList {

  public:
    /**
       Not yet documented
    */
    ValueList();

    /**
       Not yet documented
       @param value_array
    */
    ValueList(const ValueArray &value_array);

    /**
       Not yet documented
       @return
    */
    int getCount() const;

    /**
       Not yet documented
       @param value
    */
    void insertValue(const Value &value);

    /**
       Not yet documented
       @param value
    */
    void insertValueLast(const Value &value);

    /**
       Not yet documented
       @param value
    */
    void insertValueFirst(const Value &value);

    /**
       Not yet documented
       @param value
       @return
    */
    Bool suppressValue(const Value &value);

    /**
       Not yet documented
       @param value
       @return
    */
    Bool exists(const Value &value) const;

    /**
       Not yet documented
       @return
    */
    void empty();

    /**
       Not yet documented
       @return
    */
    ValueArray *toArray() const;

    ~ValueList();

  private:
    LinkedList *list;
    friend class ValueListCursor;
  };

  class ValueListCursor {

  public:
    /**
       Not yet documented
       @param valuelist
    */
    ValueListCursor(const ValueList &valuelist);

    /**
       Not yet documented
       @param valuelist
    */
    ValueListCursor(const ValueList *valuelist);

    /**
       Not yet documented
       @param value
       @return
    */
    Bool getNext(Value &value);

    ~ValueListCursor();

  private:
    LinkedListCursor *c;
  };

  /**
     @}
  */

}

#endif
