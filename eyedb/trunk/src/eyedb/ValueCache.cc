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


#include "eyedb_p.h"
#include "ValueCache.h"
#include <assert.h>

namespace eyedb {

  ValueItem::ValueItem(Object *o, const Value &v,
		       Collection::ItemId id, int state) :
    o(o), state(state), v(v), id(id), refcnt(1)
  {

    if (v.type == Value::OBJECT) {
      //printf("construct: refcnt: %p=%d\n", v.o, v.o->getRefCount());
      ObjectPeer::incrRefCount(v.o);
    }
  }

  void ValueItem::release()
  {
    if (!--refcnt) {
      if (v.type == Value::OBJECT) {
	//printf("release: refcnt: %p=%d\n", v.o, v.o->getRefCount());
	v.o->release();
      }
      delete this;
    }
  }

  void ValueItem::setState(int _state)
  {
    if (state == _state)
      return;

    state = _state;

    if (o->asCollection())
      o->asCollection()->unvalidReadCache();

    o->touch();
  }

  ValueItem::~ValueItem()
  {
  }

  ValueCache::ValueCache(Object *o) : o(o) 
  {
  }

  Status
  ValueCache::insert(const Value &v, Collection::ItemId id, int state) 
  {
    ValueItem *item = new ValueItem(o, v, id, state);
    item->incRef();

    if (val_map.find(v) != val_map.end())
      val_map[v]->release();

    val_map[v] = item;

    if (id_map.find(id) != id_map.end())
      id_map[id]->release();

    id_map[id] = item;

    if (o->asCollection())
      o->asCollection()->unvalidReadCache();

    o->touch();
    return Success;
  }

  Status ValueCache::suppress(ValueItem *item)
  {
    if (val_map.find(item->getValue()) != val_map.end()) {
      val_map[item->getValue()]->release();
      val_map.erase(val_map.find(item->getValue()));
    }
    else
      assert(0);

    if (id_map.find(item->getId()) != id_map.end()) {
      id_map[item->getId()]->release();
      id_map.erase(id_map.find(item->getId()));
    }
    else
      assert(0);
      
    if (o->asCollection())
      o->asCollection()->unvalidReadCache();

    o->touch();
    return Success;
  }

  ValueItem *ValueCache::get(Collection::ItemId id)
  {
    if (id_map.find(id) != id_map.end())
      return id_map[id];
    return 0;
  }

  ValueItem *ValueCache::get(const Value &v)
  {
    if (val_map.find(v) != val_map.end())
      return val_map[v];
    return 0;
  }

  void ValueCache::empty()
  {
    std::map<Value, ValueItem *>::iterator val_begin = val_map.begin();
    std::map<Value, ValueItem *>::iterator val_end = val_map.end();

    std::map<Value, ValueItem *>::iterator val_i = val_begin;

    while (val_i != val_end) {
      if ((*val_i).second)
	(*val_i).second->release();
      ++val_i;
    }

    val_map.erase(val_begin, val_end);

    std::map<Collection::ItemId, ValueItem *>::iterator id_begin = id_map.begin();
    std::map<Collection::ItemId, ValueItem *>::iterator id_end = id_map.end();

    std::map<Collection::ItemId, ValueItem *>::iterator id_i = id_begin;

    while (id_i != id_end) {
      if ((*id_i).second)
	(*id_i).second->release();
      ++id_i;
    }

    id_map.erase(id_begin, id_end);
  }

  ValueCache::~ValueCache()
  {
    empty();
  }

  Status
  ValueCache::setState(int state)
  {
    std::map<Value, ValueItem *>::iterator val_begin = val_map.begin();
    std::map<Value, ValueItem *>::iterator val_end = val_map.end();

    while (val_begin != val_end) {
      (*val_begin).second->setState(state);
      ++val_begin;
    }
    return Success;
  }

  ValueItem *ValueCache::get(Data data, Size item_size)
  {
    return get(Value(data, item_size));
  }
}
