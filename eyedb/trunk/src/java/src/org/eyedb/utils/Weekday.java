
//
// class Weekday
//
// package org.eyedb.utils
//
// Generated by eyedbodl at Mon May 28 11:17:03 2007
//

package org.eyedb.utils;

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

