
//
// class UserType
//
// package org.eyedb.dbm
//
// Generated by eyedbodl at Sat Dec 10 16:57:24 2005
//

package org.eyedb.dbm;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class UserType extends org.eyedb.Enum {

  UserType(org.eyedb.Database db)
  {
    super(db);
  }

  UserType()
  {
    super();
  }

  public static final int EYEDB_USER = 1;
  public static final int UNIX_USER = 2;
  public static final int STRICT_UNIX_USER = 3;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass UserType_class, org.eyedb.Schema m)
  {
    if (UserType_class == null)
      return new org.eyedb.EnumClass("user_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[3];
    en[0] = new org.eyedb.EnumItem("EYEDB_USER", 1, 0);
    en[1] = new org.eyedb.EnumItem("UNIX_USER", 2, 1);
    en[2] = new org.eyedb.EnumItem("STRICT_UNIX_USER", 3, 2);

    UserType_class.setEnumItems(en);

    return UserType_class;
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

