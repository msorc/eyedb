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

public class ProtectionMode extends org.eyedb.Enum {

  ProtectionMode(org.eyedb.Database db)
  {
    super(db);
  }

  ProtectionMode()
  {
    super();
  }

  public static final int PROT_READ = 256;
  public static final int PROT_RW = 257;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ProtectionMode_class, org.eyedb.Schema m)
  {
    if (ProtectionMode_class == null)
      return new org.eyedb.EnumClass("protection_mode");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[2];
    en[0] = new org.eyedb.EnumItem("PROT_READ", 256, 0);
    en[1] = new org.eyedb.EnumItem("PROT_RW", 257, 1);

    ProtectionMode_class.setEnumItems(en);

    return ProtectionMode_class;
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

