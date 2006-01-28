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


#ifndef _EYEDB_GBX_H
#define _EYEDB_GBX_H

#include <stdlib.h>
#include <map>

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  class LinkedList;

  class gbxTag;
  class gbxCycleContext;

  enum gbxBool {
    gbxFalse = 0,
    gbxTrue  = 1
  };

  /**
     Not yet documented.
  */
  class gbxObject {

    // ----------------------------------------------------------------------
    // gbxObject Interface
    // ----------------------------------------------------------------------
  public:
    gbxObject();
    gbxObject(const gbxTag &);
    gbxObject(const gbxObject &);
    gbxObject(const gbxObject *);

    gbxObject &operator=(const gbxObject &);

    static void *operator new(size_t);
    static void operator delete(void *);

    int getRefCount() const  {return gbx_refcnt;}
    gbxBool isLocked() const {return gbx_locked;}

    virtual void incrRefCount();
    virtual void decrRefCount();

    virtual void lock()   {gbx_locked = gbxTrue;}
    virtual void unlock() {gbx_locked = gbxFalse;}

    gbxBool isOnStack() const {return gbx_isonstack;}

    void release();

    void setTag(const gbxTag &);
    const gbxTag *getTag() const {return gbx_tag;}

    virtual void userGarbage() {}

    void keep();
    void unkeep();

    static int getObjectCount() {return obj_cnt;}
    static int getHeapSize()    {return heap_size;}

    gbxBool isValidObject() const;

    typedef std::map<gbxObject *, bool> Map;
    typedef std::map<gbxObject *, bool>::iterator MapIterator;

    static void setObjMapped(bool obj_mapped, bool reinit_if_exists);
    static bool isObjMapped();
    static gbxObject::Map *getObjMap() {return obj_map;}

    virtual ~gbxObject();

    // ----------------------------------------------------------------------
    // gbxObject Protected Part
    // ----------------------------------------------------------------------
  protected:
    int gbx_refcnt;
    gbxBool gbx_locked;
    gbxBool gbx_isonstack;
    gbxTag *gbx_tag;
    gbxBool gbx_chgRefCnt;

    void garbageRealize(gbxBool reentrant = gbxFalse);
    virtual void garbage() {}
#ifdef GBX_NEW_CYCLE
    virtual void decrRefCountPropag() {}
#endif
    virtual gbxBool grant_release() {return gbxTrue;}

    // ----------------------------------------------------------------------
    // gbxObject Private Part
    // ----------------------------------------------------------------------
  private:
    void init();
    unsigned int gbx_magic;
    gbxBool gbx_activeDestruction;
    static int obj_cnt;
    static int heap_size;
    static Map *obj_map;

    int gbx_size;
    void release_realize(gbxBool);

    // ----------------------------------------------------------------------
    // conceptually private
    // ----------------------------------------------------------------------
  public:
    virtual void manageCycle(gbxCycleContext &);
    void release_r();
    gbxBool isChgRefCnt() const {return gbx_chgRefCnt;}
    gbxBool setChgRefCnt(gbxBool chgRefCnt) {
      gbxBool old_chgRefCnt = gbx_chgRefCnt;
      gbx_chgRefCnt = chgRefCnt;
      return old_chgRefCnt;
    }
  };

  class gbxDeleter {

    // ----------------------------------------------------------------------
    // gbxDeleter Interface
    // ----------------------------------------------------------------------

  public:
    gbxDeleter(gbxObject *);

    gbxObject *keep();
    operator gbxObject *() {return o;}

    ~gbxDeleter();

  private:
    gbxBool _keep;
    gbxObject *o;
  };

  class gbxRegObj;

  class gbxAutoGarb {

    // ----------------------------------------------------------------------
    // gbxAutoGarb Interface
    // ----------------------------------------------------------------------
  public:
    enum Type {
      SUSPEND = 1,
      ACTIVE
    };

    static const int default_list_cnt = 512;
    gbxAutoGarb(int list_cnt = default_list_cnt);
    gbxAutoGarb(gbxAutoGarb::Type, int list_cnt = default_list_cnt);
    gbxAutoGarb(const gbxTag &, gbxBool excepted = gbxFalse, int list_cnt = default_list_cnt);
    gbxAutoGarb(gbxAutoGarb *);
    gbxAutoGarb(const gbxTag [], int cnt, gbxBool excepted = gbxFalse);

    gbxAutoGarb::Type suspend();
    void restore(gbxAutoGarb::Type);

    gbxAutoGarb::Type setType(gbxAutoGarb::Type);
    gbxAutoGarb::Type getType();

    void keepObjs();

    virtual ~gbxAutoGarb();

    void addObj(gbxObject *);
    gbxBool isObjRegistered(gbxObject *);
    gbxBool isObjDeleted(gbxObject *);
    gbxBool keepObj(gbxObject *, gbxBool);

    static void addObject(gbxObject *);
    static void keepObject(gbxObject *, gbxBool);
    static gbxBool isObjectRegistered(gbxObject *);
    static gbxBool isObjectDeleted(gbxObject *);
    static gbxAutoGarb *getCurrentAutoGarb() {return current_auto_garb;}

    // ----------------------------------------------------------------------
    // gbxAutoGarb Private Part
    // ----------------------------------------------------------------------
  private:
    gbxTag *tag;
    gbxBool excepted;
    gbxBool keepobjs;
    unsigned int regobjs_cnt;
    unsigned int list_cnt;
    unsigned int mask;
    LinkedList **todelete_lists;
    LinkedList **deleted_lists;
    unsigned int get_key(gbxObject *);
    gbxRegObj *find(gbxObject *o, LinkedList **);
    void wipeLists(LinkedList **);
    unsigned int countLists(LinkedList **, int state);
    unsigned int todelete_cnt;
    unsigned int deleted_cnt;
    void init(int);

    static gbxAutoGarb *current_auto_garb;
    gbxAutoGarb *prev;
    Type type;
    gbxAutoGarb *deleg_auto_garb;
    void garbage();
    gbxAutoGarb *getAutoGarb();

    // ----------------------------------------------------------------------
    // gbxAutoGarb Restrictive Part
    // ----------------------------------------------------------------------
  public: // conceptually implementation level
    static void markObjectDeleted(gbxObject *);
    gbxBool markObjDeleted(gbxObject *);
  };

  class gbxAutoGarbSuspender {

  public:
    gbxAutoGarbSuspender();
    ~gbxAutoGarbSuspender();

  private:
    gbxAutoGarb::Type type;
    gbxAutoGarb *current;
  };

  class gbxTag {

    // ----------------------------------------------------------------------
    // gbxTag Interface
    // ----------------------------------------------------------------------
  public:
    gbxTag();
    gbxTag(const gbxTag &);
    gbxTag(const char *stag);
    gbxTag(int itag);
    gbxTag(void *vtag);

    gbxTag &operator=(const gbxTag &);

    int operator==(const gbxTag &) const;
    int operator!=(const gbxTag &) const;

    const char * getSTag() const {return stag;}
    const int    getITag() const {return itag;}
    void *       getVTag() const {return vtag;}

    virtual ~gbxTag();

    // ----------------------------------------------------------------------
    // gbxTag Private Part
    // ----------------------------------------------------------------------
  private:
    char *stag;
    int  itag;
    void *vtag;
    void init();
  };

  /**
     @}
  */

}

#endif
