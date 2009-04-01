
//
// class ClassUpdateType
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Wed Apr  1 22:22:20 2009
//

package org.eyedb.syscls;

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

