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

package org.eyedb.utils;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class Weekday extends org.eyedb.Enum {

  Weekday(org.eyedb.Database db)
  {
    super(db);
  }

  Weekday()
  {
    super();
  }

  public static final int SUNDAY = 0;
  public static final int MONDAY = 1;
  public static final int TUESDAY = 2;
  public static final int WEDNESDAY = 3;
  public static final int THURSDAY = 4;
  public static final int FRIDAY = 5;
  public static final int SATURDAY = 6;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass Weekday_class, org.eyedb.Schema m)
  {
    if (Weekday_class == null)
      return new org.eyedb.EnumClass("weekday");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[7];
    en[0] = new org.eyedb.EnumItem("SUNDAY", 0, 0);
    en[1] = new org.eyedb.EnumItem("MONDAY", 1, 1);
    en[2] = new org.eyedb.EnumItem("TUESDAY", 2, 2);
    en[3] = new org.eyedb.EnumItem("WEDNESDAY", 3, 3);
    en[4] = new org.eyedb.EnumItem("THURSDAY", 4, 4);
    en[5] = new org.eyedb.EnumItem("FRIDAY", 5, 5);
    en[6] = new org.eyedb.EnumItem("SATURDAY", 6, 6);

    Weekday_class.setEnumItems(en);

    return Weekday_class;
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

