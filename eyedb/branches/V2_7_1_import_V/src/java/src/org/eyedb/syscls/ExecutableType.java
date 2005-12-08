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

public class ExecutableType extends org.eyedb.Enum {

  ExecutableType(org.eyedb.Database db)
  {
    super(db);
  }

  ExecutableType()
  {
    super();
  }

  public static final int METHOD_C_TYPE = 2;
  public static final int METHOD_OQL_TYPE = 18;
  public static final int TRIGGER_C_TYPE = 8;
  public static final int TRIGGER_OQL_TYPE = 24;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ExecutableType_class, org.eyedb.Schema m)
  {
    if (ExecutableType_class == null)
      return new org.eyedb.EnumClass("executable_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[4];
    en[0] = new org.eyedb.EnumItem("METHOD_C_TYPE", 2, 0);
    en[1] = new org.eyedb.EnumItem("METHOD_OQL_TYPE", 18, 1);
    en[2] = new org.eyedb.EnumItem("TRIGGER_C_TYPE", 8, 2);
    en[3] = new org.eyedb.EnumItem("TRIGGER_OQL_TYPE", 24, 3);

    ExecutableType_class.setEnumItems(en);

    return ExecutableType_class;
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

