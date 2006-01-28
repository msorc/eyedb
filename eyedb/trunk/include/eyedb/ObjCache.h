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


#ifndef _EYEDB_OBJ_CACHE_H
#define _EYEDB_OBJ_CACHE_H

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  struct ObjCacheLink;

  /**
     Not yet documented.
  */
  class ObjCache {

  public:
    /**
       Not yet documented
       @param n
    */
    ObjCache(int n = 8);

    /**
       Not yet documented
       @param oid
       @param o
       @return
    */
    Bool insertObject(const Oid &oid, void *o);

    /**
       Not yet documented
       @param oid
       @return
    */
    Bool deleteObject(const Oid &oid);

    /**
       Not yet documented
       @param oid
       @return
    */
    void *getObject(const Oid &oid);

    /**
       Not yet documented
       @return
    */
    ObjectList *getObjects();

    /**
       Not yet documented
    */
    void empty();

    ~ObjCache();

  private:
    unsigned int key;
    int links_cnt;
    ObjCacheLink **links;
    int getIndex(const Oid&);
    unsigned int tstamp;
  };

  /**
     @}
  */

}

#endif
