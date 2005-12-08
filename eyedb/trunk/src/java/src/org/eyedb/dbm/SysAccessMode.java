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

public class SysAccessMode extends org.eyedb.Enum {

  SysAccessMode(org.eyedb.Database db)
  {
    super(db);
  }

  SysAccessMode()
  {
    super();
  }

  public static final int NO_SYSACCESS_MODE = 0;
  public static final int DB_CREATE_SYSACCESS_MODE = 256;
  public static final int ADD_USER_SYSACCESS_MODE = 512;
  public static final int DELETE_USER_SYSACCESS_MODE = 1024;
  public static final int SET_USER_PASSWD_SYSACCESS_MODE = 2048;
  public static final int ADMIN_SYSACCESS_MODE = 768;
  public static final int SUPERUSER_SYSACCESS_MODE = 4095;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass SysAccessMode_class, org.eyedb.Schema m)
  {
    if (SysAccessMode_class == null)
      return new org.eyedb.EnumClass("system_access_mode");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[7];
    en[0] = new org.eyedb.EnumItem("NO_SYSACCESS_MODE", 0, 0);
    en[1] = new org.eyedb.EnumItem("DB_CREATE_SYSACCESS_MODE", 256, 1);
    en[2] = new org.eyedb.EnumItem("ADD_USER_SYSACCESS_MODE", 512, 2);
    en[3] = new org.eyedb.EnumItem("DELETE_USER_SYSACCESS_MODE", 1024, 3);
    en[4] = new org.eyedb.EnumItem("SET_USER_PASSWD_SYSACCESS_MODE", 2048, 4);
    en[5] = new org.eyedb.EnumItem("ADMIN_SYSACCESS_MODE", 768, 5);
    en[6] = new org.eyedb.EnumItem("SUPERUSER_SYSACCESS_MODE", 4095, 6);

    SysAccessMode_class.setEnumItems(en);

    return SysAccessMode_class;
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

