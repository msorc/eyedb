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

namespace eyedb {
  struct ObjCacheLink {
    Oid oid;
    void *o;
    unsigned int tstamp;
    ObjCacheLink *next;
    ObjCacheLink(const Oid &, void *, unsigned int);
  };

ObjCacheLink::ObjCacheLink(const Oid &toid, void *to, unsigned int ts)
{
  oid = toid;
  o = to;
  tstamp = ts;
}

ObjCache::ObjCache(int n)
{
/*
  key = 1;

  while (n--)
    key <<= 1;

  --key;
*/
  key = n - 1;

  links_cnt = key + 1;
  links = (ObjCacheLink **)malloc(sizeof(ObjCacheLink *) * links_cnt);
  memset(links, 0, sizeof(ObjCacheLink*) * links_cnt);
  tstamp = 0;
}

inline int ObjCache::getIndex(const Oid &oid)
{
  return (oid.getNX() & key);
}

Bool ObjCache::insertObject(const Oid &oid, void *o)
{
//  printf("insertObject %s o = %p links %p\n", oid.getString(), o, links);
  if (getObject(oid))
    return False;

  int index = getIndex(oid);
  ObjCacheLink *ol = new ObjCacheLink(oid, o, ++tstamp);
  ObjCacheLink *link = links[index];

  ol->next = link;
  links[index] = ol;

  return True;
}

Bool ObjCache::deleteObject(const Oid &oid)
{
  ObjCacheLink *prev = 0;
  ObjCacheLink *link = links[getIndex(oid)];

//  printf("deleteObject %s links %p\n", oid.getString(), links);
  while (link)
    {
      if (oid.compare(link->oid))
	{
	  if (prev)
	    prev->next = link->next;
	  else
	    links[getIndex(oid)] = link->next;

	  delete link;
	  return True;
	}
      prev = link;
      link = link->next;
    }

  return False;
}

void *ObjCache::getObject(const Oid &oid)
{
  ObjCacheLink *link = links[getIndex(oid)];

//  printf("getObject %s links %p\n", oid.getString(), links);
  while (link)
    {
      if (oid.compare(link->oid))
	{
//	  printf("returns %p\n", link->o);
	  return link->o;
	}
      link = link->next;
    }

//  printf("returns 0\n");
  return 0;
}

ObjectList *
ObjCache::getObjects()
{
  ObjectList *obj_list = new ObjectList();
  for (int n = 0; n < links_cnt; n++)
    {
      ObjCacheLink *link = links[n];
      while (link)
	{
	  obj_list->insertObjectLast((Object *)link->o);
	  link = link->next;
	}
    }

  return obj_list;
}

void ObjCache::empty(void)
{
  for (int n = 0; n < links_cnt; n++)
    {
      ObjCacheLink *link = links[n];

      while (link)
	{
	  ObjCacheLink *next = link->next;
	  delete link;
	  link = next;
	}

      links[n] = 0;
    }

  tstamp = 0;
}

ObjCache::~ObjCache()
{
  empty();
  free(links);
}
}
