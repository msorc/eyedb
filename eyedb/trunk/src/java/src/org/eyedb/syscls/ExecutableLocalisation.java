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

public class ExecutableLocalisation extends org.eyedb.Enum {

  ExecutableLocalisation(org.eyedb.Database db)
  {
    super(db);
  }

  ExecutableLocalisation()
  {
    super();
  }

  public static final int BACKEND = 1;
  public static final int FRONTEND = 2;
  public static final int STATIC_EXEC = 256;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ExecutableLocalisation_class, org.eyedb.Schema m)
  {
    if (ExecutableLocalisation_class == null)
      return new org.eyedb.EnumClass("executable_localisation");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[3];
    en[0] = new org.eyedb.EnumItem("BACKEND", 1, 0);
    en[1] = new org.eyedb.EnumItem("FRONTEND", 2, 1);
    en[2] = new org.eyedb.EnumItem("STATIC_EXEC", 256, 2);

    ExecutableLocalisation_class.setEnumItems(en);

    return ExecutableLocalisation_class;
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

