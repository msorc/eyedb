
//
// class set_class_AttributeComponent_ref
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Fri Jan 18 22:55:25 2008
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class set_class_AttributeComponent_ref extends org.eyedb.CollSetClass {

  private set_class_AttributeComponent_ref(org.eyedb.Class coll_class, boolean isref) {
    super(coll_class, isref);
  }

  public static org.eyedb.Class idbclass;

  static org.eyedb.CollSetClass make(org.eyedb.CollSetClass cls, org.eyedb.Schema m)
  {
    if (cls == null)
      {
      cls = new org.eyedb.CollSetClass(((m != null) ? m.getClass("AttributeComponent") : AttributeComponent.idbclass), true);
    }
    return cls;
  }

  static void init_p()
  {
    idbclass = make(null, null);
  }
}

