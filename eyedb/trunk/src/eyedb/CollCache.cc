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

#ifndef USE_VALUE_CACHE
#include "CollCache.h"
#include <assert.h>

//
// CollItem methods
//

namespace eyedb {

  void CollItem::init(Collection *_coll, Collection::ItemId _id, int _st)
  {
    id = _id;
    state = _st;
    coll = _coll;

    data = (Data)0;
    o = NULL;
    is_oid = is_object = is_value = is_data = False;
  }

  CollItem::CollItem(Collection *_coll, const Oid& _oid, Collection::ItemId _id, int _st)
  {
    init(_coll, _id, _st);
    oid = _oid;
    is_oid = True;
  }

  CollItem::CollItem(Collection *_coll, const Object *_o, Collection::ItemId _id, int _st)
  {
    init(_coll, _id, _st);
    o = _o;
    is_object = True;
  }

  CollItem::CollItem(Collection *_coll, const Value &_value, Collection::ItemId _id, int _st)
  {
    init(_coll, _id, _st);
    value = _value;
    is_value = True;
  }

  CollItem::CollItem(Collection *_coll, Data _data, Collection::ItemId _id, int _st)
  {
    init(_coll, _id, _st);
    data = _data;
    is_data = True;
  }

  /*
    void CollItem::setOid(const Oid& _oid)
    {
    oid = _oid;
    }
  */

  const Oid& CollItem::getOid() const
  {
    if (is_oid)
      return oid;
    if (is_object)
      return o ? o->getOid() : Oid::nullOid;
    if (is_value)
      return value.oid ? *value.oid : Oid::nullOid;

    return Oid::nullOid;
  }

  /*
    void  CollItem::setObject(const Object *_o)
    {
    o = _o;
    }
  */

  const Object *CollItem::getObject() const
  {
    if (is_object)
      return o;
    if (is_oid)
      return 0;
    if (is_value)
      return value.o;

    return 0;
  }

  /*
    void CollItem::setData(Data _data)
    {
    data = _data;
    }
  */

  Data CollItem::getData() const
  {
    if (is_data)
      return data;
    return 0;
  }

  /*
    void CollItem::setValue(const Value &_value)
    {
    value = _value;
    }
  */

  const Value &CollItem::getValue() const
  {
    if (is_oid)
      const_cast<CollItem *>(this)->value = Value(oid);
    else if (is_object)
      const_cast<CollItem *>(this)->value = Value(o);

    return value;
  }

  void
  CollItem::setState(int _state)
  {
    if (state == _state)
      return;

    coll->unvalidReadCache();
#ifdef IDB_COLLMOD_OPT1
    coll->touch();
#endif
    state = _state;
  }

  //
  // CollCache methods
  // 

  void CollCache::init()
  {
    list = new LinkedList();

    oid_list   = (LinkedList **)calloc(nkeys * sizeof(LinkedList *), 1);
    id_list    = (LinkedList **)calloc(nkeys * sizeof(LinkedList *), 1);
    obj_list   = (LinkedList **)calloc(nkeys * sizeof(LinkedList *), 1);
    data_list  = (LinkedList **)calloc(nkeys * sizeof(LinkedList *), 1);
    value_list = (LinkedList **)calloc(nkeys * sizeof(LinkedList *), 1);

    nitems = 0;
  }

  void
  CollCache::setColl(Collection *_coll)
  {
    coll = _coll;
    CollItem *item;
    LinkedListCursor c(list);
    while (c.getNext((void *&)item))
      item->setColl(_coll);
  }

  CollCache::CollCache(Collection *_coll, unsigned int magorder,
		       Size isz, unsigned int max)
  {
    mask = 2;
    unsigned int h1 = magorder / 128;
    if (h1 > 512) h1 = 512;
    for (;;) {
      if (mask >= h1)
	break;
      mask <<= 1;
    }

    nkeys = mask--;

    item_size = isz;
    max_items = max;
    init();
    coll = _coll;
    //printf("CollCache::CollCache(this=%p, coll=%p, mask=%x, nkeys=%d)\n", this, coll, mask, nkeys);
  }

  LinkedList *CollCache::getList()
  {
    return list;
  }

  Status CollCache::insert_realize(const void *v, int st,
				   Collection::ItemId id,
				   unsigned int key,
				   LinkedList **tlist)
  {
    CollItem *item;

    if (tlist == oid_list)
      item = new CollItem(coll, *(Oid *)v, id, st);
    else if (tlist == obj_list)
      {
	item = new CollItem(coll, (const Object *)v, id, st);
	ObjectPeer::incrRefCount((Object *)v);
      }
    else if (tlist == data_list)
      item = new CollItem(coll, (Data)v, id, st);
    else if (tlist == value_list)
      item = new CollItem(coll, *(Value *)v, id, st);
    else
      assert(0);

    /* insert in the tlist */
    if (!tlist[key])
      tlist[key] = new LinkedList();

    tlist[key]->insertObject(item);

    /* insert in the id list */
    key = key_id(id);

    if (!id_list[key])
      id_list[key] = new LinkedList();

    id_list[key]->insertObject(item);

    /* insert in the global list */
    list->insertObject(item);

    coll->unvalidReadCache();
#ifdef IDB_COLLMOD_OPT1
    coll->touch();
#endif
    return Success;
  }

  Status CollCache::insert(const Oid& oid, Collection::ItemId id, int st)
  {
    return insert_realize((const void *)&oid, st, id, key_oid(oid), oid_list);
  }

  Status CollCache::insert(const Object *o, Collection::ItemId id, int st)
  {
    return insert_realize((const void *)o, st, id, key_obj(o), obj_list);
  }
  
  Status CollCache::insert(Data data, Collection::ItemId id, int st)
  {
    return insert_realize((const void *)data, st, id, key_data(data), data_list);
  }
  
  Status CollCache::insert(const Value &value, Collection::ItemId id, int st)
  {
    return insert_realize((const void *)&value, st, id, key_value(value),
			  value_list);
  }
  
  Status
  CollCache::suppress(CollItem *item)
  {
    Collection::ItemId id = item->getId();

    int key = key_id(id);
  
    if (id_list[key])
      id_list[key]->deleteObject(item);

    list->deleteObject(item);

    delete item;

    coll->unvalidReadCache();
#ifdef IDB_COLLMOD_OPT1
    coll->touch();
#endif
    return Success;
  }

  Status CollCache::suppressOid(CollItem *item)
  {
    Oid item_oid = item->getOid();

    unsigned int key = key_oid(item_oid);
      
    if (oid_list[key])
      oid_list[key]->deleteObject(item);

    return suppress(item);
  }

  Status CollCache::suppressObject(CollItem *item)
  {
    const Object *item_o;

    if (item_o = item->getObject())
      {
	int key = key_obj(item_o);

	if (obj_list[key])
	  obj_list[key]->deleteObject(item);
      }

    return suppress(item);
  }

  Status CollCache::suppressData(CollItem *item)
  {
    Data item_data = item->getData();

    int key = key_data(item_data);
      
    if (data_list[key])
      data_list[key]->deleteObject(item);

    return suppress(item);
  }

  Status CollCache::suppressValue(CollItem *item)
  {
    Value item_value = item->getValue();

    int key = key_value(item_value);
      
    if (value_list[key])
      value_list[key]->deleteObject(item);

    return suppress(item);
  }

  CollItem *CollCache::get(Collection::ItemId id)
  {
    CollItem *item;
    int key;

    key = key_id(id);

    LinkedList *l;
    if (l = id_list[key])
      {
	LinkedListCursor c(l);

	while (c.getNext((void* &)item))
	  if (item->getId() == id)
	    return item;
      }

    return 0;
  }

  CollItem *CollCache::get(const Object *o)
  {
    CollItem *item;
    int key;

    key = key_obj(o);

    LinkedList *l;
    if (l = obj_list[key])
      {
	LinkedListCursor c(l);

	while (c.getNext((void* &)item))
	  if (item->getObject() == o)
	    return item;
      }

    return o->getOid().isValid() ? get(o->getOid()) : 0;
  }

  CollItem *CollCache::get(const Oid& _oid)
  {
    CollItem *item;
    unsigned int key;

    key = key_oid(_oid);

    LinkedList *l;
    if (l = oid_list[key])
      {
	LinkedListCursor c(l);

	while (c.getNext((void* &)item))
	  if (item->getOid().compare(_oid))
	    return item;
      }

    return 0;
  }

  CollItem *CollCache::get(Data data)
  {
    CollItem *item;
    int key;

    key = key_data(data);

    LinkedList *l;
    if (l = data_list[key])
      {
	LinkedListCursor c(l);

	while (c.getNext((void* &)item))
	  if (!memcmp(item->getData(), data, item_size))
	    return item;
      }

    return 0;
  }

  CollItem *CollCache::get(const Value &value)
  {
    CollItem *item;
    int key;

    key = key_value(value);

    LinkedList *l;
    if (l = value_list[key])
      {
	LinkedListCursor c(l);

	while (c.getNext((void* &)item))
	  if (item->getValue() == value)
	    return item;
      }

    return 0;
  }

  void CollCache::remove(LinkedList **l)
  {
    for (int i = 0; i < nkeys; i++)
      delete l[i];
    free(l);
  }

  void CollCache::empty(Bool r)
  {
    LinkedListCursor c(list);
    int i;
    CollItem *item;

    while (c.getNext((void* &)item))
      {
	if (item->getObject())
	  ((Object *)item->getObject())->release();
	delete item;
      }

    delete list;

    remove(id_list);
    remove(oid_list);
    remove(obj_list);
    remove(value_list);
    remove(data_list);

    if (!r)
      init();
  }

  CollCache::~CollCache()
  {
    //printf("CollCache::~CollCache(this=%p, coll=%p)\n", this, coll);
    empty(True);
  }
}
#endif
