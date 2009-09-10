
//
// class ExecutableType
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Thu Sep 10 15:18:45 2009
//

package org.eyedb.syscls;

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

