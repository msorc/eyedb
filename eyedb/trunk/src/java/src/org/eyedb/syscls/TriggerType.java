
//
// class TriggerType
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Thu Sep 10 15:18:45 2009
//

package org.eyedb.syscls;

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

