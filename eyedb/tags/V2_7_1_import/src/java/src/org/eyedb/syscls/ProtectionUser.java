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

public class ProtectionUser extends Root {

  public ProtectionUser(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public ProtectionUser(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("protection_user") : ProtectionUser.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public ProtectionUser(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public ProtectionUser(ProtectionUser x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setUser(org.eyedb.Object _user)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_user), 0);
  }

  public org.eyedb.Object getUser()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[2].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(__y), 0);
    return (org.eyedb.Object)__y;
  }

  public void setUserOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[2].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getUserOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[2].getOid(this, 0);
    return __x;
  }

  public void setMode(int _mode)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_mode), 0);
  }

  public int getMode()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[3].getValue(this, 0, true);
    return __x.sgetInt();
  }


  protected ProtectionUser(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected ProtectionUser(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected ProtectionUser(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected ProtectionUser(ProtectionUser x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass ProtectionUser_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (ProtectionUser_class == null)
      return new StructClass("protection_user", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[4];
    int[] dims;

    dims = null;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("object") : org.eyedb.Object.idbclass), idbclass, "user", 2, true, 0, dims);

    dims = null;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("protection_mode") : ProtectionMode.idbclass), idbclass, "mode", 3, false, 0, dims);

    ProtectionUser_class.setAttributes(attr, 2, 2);

    return ProtectionUser_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("protection_user", ProtectionUser.class.getConstructor(Database.clazz));
    } catch(java.lang.Exception e) {
      e.printStackTrace();
    }
  }

  static void init()
   throws org.eyedb.Exception {
    make((StructClass)idbclass, null);

    idr_objsz = idbclass.getObjectSize();
    idr_psize = idbclass.getObjectPSize();
  }

}

