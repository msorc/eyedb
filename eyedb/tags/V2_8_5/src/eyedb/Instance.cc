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

#include <eyedb/eyedb.h>

namespace eyedb {

  Instance::Instance(Database *_db, const Dataspace *_dataspace) :
    Object(_db, _dataspace)
  {
    //  Object::initialize(_db); // conceptually is Instance::initialize(_db)
  }

  void Instance::initialize(Database *_db)
  {
    Object::initialize(_db);
  }

  void Instance::garbage()
  {
    Object::garbage();
  }

  Instance::~Instance()
  {
    garbageRealize();
  }

}
