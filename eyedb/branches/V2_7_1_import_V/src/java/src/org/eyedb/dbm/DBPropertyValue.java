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

package org.eyedb.dbm;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class DBPropertyValue extends Root {

  public DBPropertyValue(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public DBPropertyValue(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("database_property_value") : DBPropertyValue.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public DBPropertyValue(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public DBPropertyValue(DBPropertyValue x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setIval(long _ival)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_ival), 0);
  }

  public long getIval()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[2].getValue(this, 0, true);
    return __x.sgetLong();
  }

  public void setSval(String _sval)
  throws org.eyedb.Exception {
    int len = _sval.length() + 1;

    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[3].setSize(this, len);
    getClass(true).getAttributes()[3].setStringValue(this, _sval);
  }

  public void setSval(int a0, char _sval)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[3].setSize(this, from+1);
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_sval), from);
  }

  public String getSval()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[3].getStringValue(this);
  }

  public void setBval(byte _bval[])
  throws org.eyedb.Exception {
    int len = _bval.length;

    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[4].setSize(this, len);
    getClass(true).getAttributes()[4].setRawValue(this, _bval);
  }

  public void setBval(int a0, byte _bval)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[4].setSize(this, from+1);
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_bval), from);
  }

  public byte[] getBval()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[4].getRawValue(this);
  }

  public void setOval(org.eyedb.Object _oval)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_oval), 0);
  }

  public org.eyedb.Object getOval()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[5].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(__y), 0);
    return (org.eyedb.Object)__y;
  }

  public void setOvalOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[5].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getOvalOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[5].getOid(this, 0);
    return __x;
  }


  protected DBPropertyValue(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected DBPropertyValue(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected DBPropertyValue(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected DBPropertyValue(DBPropertyValue x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass DBPropertyValue_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (DBPropertyValue_class == null)
      return new StructClass("database_property_value", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[6];
    int[] dims;

    dims = null;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("long") : Int64.idbclass), idbclass, "ival", 2, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "sval", 3, false, 1, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("byte") : org.eyedb.Byte.idbclass), idbclass, "bval", 4, false, 1, dims);

    dims = null;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("object") : org.eyedb.Object.idbclass), idbclass, "oval", 5, true, 0, dims);

    DBPropertyValue_class.setAttributes(attr, 2, 4);

    return DBPropertyValue_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("database_property_value", DBPropertyValue.class.getConstructor(Database.clazz));
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

