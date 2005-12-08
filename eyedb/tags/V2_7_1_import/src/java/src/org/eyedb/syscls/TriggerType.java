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

public class TriggerType extends org.eyedb.Enum {

  TriggerType(org.eyedb.Database db)
  {
    super(db);
  }

  TriggerType()
  {
    super();
  }

  public static final int TRIGGER_CREATE_BEFORE = 17;
  public static final int TRIGGER_CREATE_AFTER = 18;
  public static final int TRIGGER_UPDATE_BEFORE = 33;
  public static final int TRIGGER_UPDATE_AFTER = 34;
  public static final int TRIGGER_LOAD_BEFORE = 65;
  public static final int TRIGGER_LOAD_AFTER = 66;
  public static final int TRIGGER_REMOVE_BEFORE = 129;
  public static final int TRIGGER_REMOVE_AFTER = 130;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass TriggerType_class, org.eyedb.Schema m)
  {
    if (TriggerType_class == null)
      return new org.eyedb.EnumClass("trigger_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[8];
    en[0] = new org.eyedb.EnumItem("TRIGGER_CREATE_BEFORE", 17, 0);
    en[1] = new org.eyedb.EnumItem("TRIGGER_CREATE_AFTER", 18, 1);
    en[2] = new org.eyedb.EnumItem("TRIGGER_UPDATE_BEFORE", 33, 2);
    en[3] = new org.eyedb.EnumItem("TRIGGER_UPDATE_AFTER", 34, 3);
    en[4] = new org.eyedb.EnumItem("TRIGGER_LOAD_BEFORE", 65, 4);
    en[5] = new org.eyedb.EnumItem("TRIGGER_LOAD_AFTER", 66, 5);
    en[6] = new org.eyedb.EnumItem("TRIGGER_REMOVE_BEFORE", 129, 6);
    en[7] = new org.eyedb.EnumItem("TRIGGER_REMOVE_AFTER", 130, 7);

    TriggerType_class.setEnumItems(en);

    return TriggerType_class;
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

