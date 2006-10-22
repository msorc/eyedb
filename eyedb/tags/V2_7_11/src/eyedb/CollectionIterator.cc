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


#include "eyedb_p.h"

namespace eyedb {

  CollectionIterator::CollectionIterator(const Collection *_coll, Bool indexed)
  {
    init(_coll, indexed);
  }

  CollectionIterator::CollectionIterator(const Collection &_coll, Bool indexed)
  {
    init(&_coll, indexed);
  }

  void CollectionIterator::init(const Collection *_coll, Bool indexed)
  {
    coll = _coll;
    if (coll->isPartiallyStored()) {
      q = 0;
      cur = 0;
      status = coll->getElements(val_arr);
      return;
    }

    q = new Iterator(coll, indexed);
    status = q->getStatus();
    if (status) {
      delete q;
      q = 0;
    }
  }

  Bool CollectionIterator::next(Oid &oid)
  {
    if (status)
      throw *status;

    if (!q) {
      for (int n = cur; cur < val_arr.getCount(); cur++) {
	Value &v = val_arr[cur];
	if (v.type == Value::tObject) {
	  if (v.o)
	    oid = v.o->getOid();
	  else
	    oid.invalidate();
	  cur++;
	  return True;
	}

	if (v.type == Value::tOid) {
	  oid = *v.oid;
	  return True;
	}
      }

      return False;
    }

    Bool found;
    status = q->scanNext(found, oid);
    if (status)
      throw *status;
    return found;
  }

  Bool CollectionIterator::next(Object *&o, const RecMode *rcm)
  {
    if (status)
      throw *status;

    if (!q) {
      for (int n = cur; cur < val_arr.getCount(); cur++) {
	Value &v = val_arr[cur];
	if (v.type == Value::tObject) {
	  o = v.o;
	  cur++;
	  return True;
	}

	if (v.type == Value::tOid && coll->getDatabase()) {
	  status = const_cast<Database *>(coll->getDatabase())->loadObject(*v.oid, o, rcm);
	  if (status)
	    throw *status;
	  return True;
	}
      }

      return False;
    }

    Bool found;
    o = 0;
    status = q->scanNext(found, o, rcm);
    if (status) 
      throw *status;

    return found;
  }

  Bool CollectionIterator::next(Value &v)
  {
    if (status)
      throw *status;

    if (!q) {
      if (cur < val_arr.getCount()) {
	v = val_arr[cur++];
	return True;
      }
      return False;
    }

    Bool found;
    status = q->scanNext(found, v);
    if (status)
      throw *status;

    const_cast<Collection *>(coll)->makeValue(v);
    return found;
  }

  CollectionIterator::~CollectionIterator()
  {
    if (!q) {
      for (int n = cur; cur < val_arr.getCount(); cur++) {
	Value &v = val_arr[cur];
	if (v.type == Value::tObject && v.o)
	  v.o->release();
      }
    }

    delete q;
  }
}
