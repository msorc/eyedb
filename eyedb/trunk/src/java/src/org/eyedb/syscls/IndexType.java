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

package org.eyedb.syscls;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class IndexType extends org.eyedb.Enum {

  IndexType(org.eyedb.Database db)
  {
    super(db);
  }

  IndexType()
  {
    super();
  }

  public static final int HASH_INDEX_TYPE = 32;
  public static final int BTREE_INDEX_TYPE = 64;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass IndexType_class, org.eyedb.Schema m)
  {
    if (IndexType_class == null)
      return new org.eyedb.EnumClass("index_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[2];
    en[0] = new org.eyedb.EnumItem("HASH_INDEX_TYPE", 32, 0);
    en[1] = new org.eyedb.EnumItem("BTREE_INDEX_TYPE", 64, 1);

    IndexType_class.setEnumItems(en);

    return IndexType_class;
  }

  static void init_p()
  {
    idbclass = make(null, null);
  }

  static void init()
  {
    make((org.eyedb.EnumClass)idbclass, null);
  }
  public static org.eyedb.Class idbclass;
}

