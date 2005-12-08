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

public class ClassUpdateType extends org.eyedb.Enum {

  ClassUpdateType(org.eyedb.Database db)
  {
    super(db);
  }

  ClassUpdateType()
  {
    super();
  }

  public static final int ADD_ATTR = 0;
  public static final int RMV_ATTR = 1;
  public static final int CNV_ATTR = 2;
  public static final int MIG_ATTR = 3;
  public static final int RMV_CLASS = 4;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ClassUpdateType_class, org.eyedb.Schema m)
  {
    if (ClassUpdateType_class == null)
      return new org.eyedb.EnumClass("class_update_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[5];
    en[0] = new org.eyedb.EnumItem("ADD_ATTR", 0, 0);
    en[1] = new org.eyedb.EnumItem("RMV_ATTR", 1, 1);
    en[2] = new org.eyedb.EnumItem("CNV_ATTR", 2, 2);
    en[3] = new org.eyedb.EnumItem("MIG_ATTR", 3, 3);
    en[4] = new org.eyedb.EnumItem("RMV_CLASS", 4, 4);

    ClassUpdateType_class.setEnumItems(en);

    return ClassUpdateType_class;
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

