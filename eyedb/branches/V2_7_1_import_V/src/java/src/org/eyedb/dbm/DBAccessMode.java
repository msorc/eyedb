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

public class DBAccessMode extends org.eyedb.Enum {

  DBAccessMode(org.eyedb.Database db)
  {
    super(db);
  }

  DBAccessMode()
  {
    super();
  }

  public static final int NO_DBACCESS_MODE = 0;
  public static final int READ_DBACCESS_MODE = 16;
  public static final int WRITE_DBACCESS_MODE = 32;
  public static final int EXEC_DBACCESS_MODE = 64;
  public static final int READ_WRITE_DBACCESS_MODE = 48;
  public static final int READ_EXEC_DBACCESS_MODE = 80;
  public static final int READ_WRITE_EXEC_DBACCESS_MODE = 112;
  public static final int ADMIN_DBACCESS_MODE = 113;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass DBAccessMode_class, org.eyedb.Schema m)
  {
    if (DBAccessMode_class == null)
      return new org.eyedb.EnumClass("database_access_mode");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[8];
    en[0] = new org.eyedb.EnumItem("NO_DBACCESS_MODE", 0, 0);
    en[1] = new org.eyedb.EnumItem("READ_DBACCESS_MODE", 16, 1);
    en[2] = new org.eyedb.EnumItem("WRITE_DBACCESS_MODE", 32, 2);
    en[3] = new org.eyedb.EnumItem("EXEC_DBACCESS_MODE", 64, 3);
    en[4] = new org.eyedb.EnumItem("READ_WRITE_DBACCESS_MODE", 48, 4);
    en[5] = new org.eyedb.EnumItem("READ_EXEC_DBACCESS_MODE", 80, 5);
    en[6] = new org.eyedb.EnumItem("READ_WRITE_EXEC_DBACCESS_MODE", 112, 6);
    en[7] = new org.eyedb.EnumItem("ADMIN_DBACCESS_MODE", 113, 7);

    DBAccessMode_class.setEnumItems(en);

    return DBAccessMode_class;
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

