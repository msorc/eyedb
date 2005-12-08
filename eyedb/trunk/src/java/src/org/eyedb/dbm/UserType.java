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

package org.eyedb.dbm;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class UserType extends org.eyedb.Enum {

  UserType(org.eyedb.Database db)
  {
    super(db);
  }

  UserType()
  {
    super();
  }

  public static final int EYEDB_USER = 1;
  public static final int UNIX_USER = 2;
  public static final int STRICT_UNIX_USER = 3;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass UserType_class, org.eyedb.Schema m)
  {
    if (UserType_class == null)
      return new org.eyedb.EnumClass("user_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[3];
    en[0] = new org.eyedb.EnumItem("EYEDB_USER", 1, 0);
    en[1] = new org.eyedb.EnumItem("UNIX_USER", 2, 1);
    en[2] = new org.eyedb.EnumItem("STRICT_UNIX_USER", 3, 2);

    UserType_class.setEnumItems(en);

    return UserType_class;
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

