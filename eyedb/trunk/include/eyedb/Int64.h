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


#ifndef _EYEDB_INT64_H
#define _EYEDB_INT64_H

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  /**
     Not yet documented.
  */
  class Int64 : public Basic {

    // -----------------------------------------------------------------------
    // Int64 Interface
    // -----------------------------------------------------------------------
  public:
    /**
       Not yet documented
       @param i
    */
    Int64(eyedblib::int64 i = 0);

    /**
       Not yet documented
       @param db
       @param i
       @param dataspace
    */
    Int64(Database *db, eyedblib::int64 i = 0, const Dataspace *dataspace = 0);

    /**
       Not yet documented
       @param o
    */
    Int64(const Int64 *o);

    /**
       Not yet documented
       @param o
    */
    Int64(const Int64 &o);

    /**
       Not yet documented
       @return
    */
    virtual Object *clone() const {return new Int64(*this);}

    /**
       Not yet documented
       @param o
       @return
    */
    Int64& operator=(const Int64 &o);

    /**
       Not yet documented
       @param fd
       @param flags
       @param recmode
       @return
    */
    Status trace(FILE *fd = stdout, unsigned int flags = 0, const RecMode *recmode = RecMode::FullRecurs) const;

    /**
       Not yet documented
       @param data
       @return
    */
    Status setValue(Data data);

    /**
       Not yet documented
       @param data
       @return
    */
    Status getValue(Data *data) const;

    /**
       Not yet documented
       @return
    */
    virtual Int64 *asInt64() {return this;}

    /**
       Not yet documented
       @return
    */
    virtual const Int64 *asInt64() const {return this;}

    // -----------------------------------------------------------------------
    // Int64 Private Part
    // -----------------------------------------------------------------------
  private:
    Status trace_realize(FILE*, int, unsigned int, const RecMode *) const;
    eyedblib::int64 val;
    Status create();
    Status update();
  };

  extern Int64Class   *Int64_Class;
  extern const char int64_class_name[];

  /**
     @}
  */

}

#endif
