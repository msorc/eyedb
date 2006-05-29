
//
// class ExecutableLang
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Sat Dec 10 16:57:24 2005
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class ExecutableLang extends org.eyedb.Enum {

  ExecutableLang(org.eyedb.Database db)
  {
    super(db);
  }

  ExecutableLang()
  {
    super();
  }

  public static final int C_LANG = 1;
  public static final int OQL_LANG = 2;
  public static final int SYSTEM_EXEC = 256;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ExecutableLang_class, org.eyedb.Schema m)
  {
    if (ExecutableLang_class == null)
      return new org.eyedb.EnumClass("executable_lang");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[3];
    en[0] = new org.eyedb.EnumItem("C_LANG", 1, 0);
    en[1] = new org.eyedb.EnumItem("OQL_LANG", 2, 1);
    en[2] = new org.eyedb.EnumItem("SYSTEM_EXEC", 256, 2);

    ExecutableLang_class.setEnumItems(en);

    return ExecutableLang_class;
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

