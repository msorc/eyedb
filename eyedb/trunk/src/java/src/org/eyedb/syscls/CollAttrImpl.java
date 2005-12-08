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

public class CollAttrImpl extends AttributeComponent {

  public CollAttrImpl(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public CollAttrImpl(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("collection_attribute_implementation") : CollAttrImpl.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CollAttrImpl(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CollAttrImpl(CollAttrImpl x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setIdxtype(int _idxtype)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[6].setValue(this, new org.eyedb.Value(_idxtype), 0);
  }

  public int getIdxtype()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[6].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setDspid(short _dspid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[7].setValue(this, new org.eyedb.Value(_dspid), 0);
  }

  public short getDspid()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[7].getValue(this, 0, true);
    return __x.sgetShort();
  }

  public void setKeyCountOrDegree(int _key_count_or_degree)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[8].setValue(this, new org.eyedb.Value(_key_count_or_degree), 0);
  }

  public int getKeyCountOrDegree()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[8].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setHashMethod(BEMethod_C _hash_method)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[9].setValue(this, new org.eyedb.Value(_hash_method), 0);
  }

  public BEMethod_C getHashMethod()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[9].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[9].setValue(this, new org.eyedb.Value(__y), 0);
    return (BEMethod_C)__y;
  }

  public void setHashMethodOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[9].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getHashMethodOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[9].getOid(this, 0);
    return __x;
  }

  public void setImplHints(int a0, int _impl_hints)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[10].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[10].setSize(this, from+1);
    getClass(true).getAttributes()[10].setValue(this, new org.eyedb.Value(_impl_hints), from);
  }

  public int getImplHints(int a0)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;
    int from = a0;

    __x = getClass(true).getAttributes()[10].getValue(this, from, true);
    return __x.sgetInt();
  }

  int getImpl_hints_cnt()
  {
    return getClass(true).getAttributes()[10].getSize(this);
  }


  protected CollAttrImpl(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected CollAttrImpl(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected CollAttrImpl(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected CollAttrImpl(CollAttrImpl x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass CollAttrImpl_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (CollAttrImpl_class == null)
      return new StructClass("collection_attribute_implementation", ((m != null) ? m.getClass("AttributeComponent") : AttributeComponent.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[11];
    int[] dims;

    dims = null;
    attr[6] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "idxtype", 6, false, 0, dims);

    dims = null;
    attr[7] = new org.eyedb.Attribute(((m != null) ? m.getClass("short") : Int16.idbclass), idbclass, "dspid", 7, false, 0, dims);

    dims = null;
    attr[8] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "key_count_or_degree", 8, false, 0, dims);

    dims = null;
    attr[9] = new org.eyedb.Attribute(((m != null) ? m.getClass("be_method_C") : BEMethod_C.idbclass), idbclass, "hash_method", 9, true, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[10] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "impl_hints", 10, false, 1, dims);

    CollAttrImpl_class.setAttributes(attr, 6, 5);

    return CollAttrImpl_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("collection_attribute_implementation", CollAttrImpl.class.getConstructor(Database.clazz));
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

