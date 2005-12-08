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

public class set_class_AttributeComponent_ref extends org.eyedb.CollSetClass {

  private set_class_AttributeComponent_ref(org.eyedb.Class coll_class, boolean isref) {
    super(coll_class, isref);
  }

  public static org.eyedb.Class idbclass;

  static CollSetClass make(CollSetClass cls, org.eyedb.Schema m)
  {
    if (cls == null)
      {
      cls = new CollSetClass(((m != null) ? m.getClass("AttributeComponent") : AttributeComponent.idbclass), true);
    }
    return cls;
  }

  static void init_p()
  {
    idbclass = make(null, null);
  }
}

