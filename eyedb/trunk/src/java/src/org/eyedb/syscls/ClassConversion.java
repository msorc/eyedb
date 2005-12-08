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

public class ClassConversion extends Root {

  public ClassConversion(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public ClassConversion(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("class_conversion") : ClassConversion.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public ClassConversion(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public ClassConversion(ClassConversion x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setUpdtype(int _updtype)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_updtype), 0);
  }

  public int getUpdtype()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[2].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setCnvtype(int _cnvtype)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_cnvtype), 0);
  }

  public int getCnvtype()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[3].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setClsname(String _clsname)
  throws org.eyedb.Exception {
    int len = _clsname.length() + 1;

    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[4].setSize(this, len);
    getClass(true).getAttributes()[4].setStringValue(this, _clsname);
  }

  public void setClsname(int a0, char _clsname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[4].setSize(this, from+1);
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_clsname), from);
  }

  public String getClsname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[4].getStringValue(this);
  }

  public void setAttrname(String _attrname)
  throws org.eyedb.Exception {
    int len = _attrname.length() + 1;

    int size = getClass(true).getAttributes()[5].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[5].setSize(this, len);
    getClass(true).getAttributes()[5].setStringValue(this, _attrname);
  }

  public void setAttrname(int a0, char _attrname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[5].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[5].setSize(this, from+1);
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_attrname), from);
  }

  public String getAttrname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[5].getStringValue(this);
  }

  public void setAttrnum(int _attrnum)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[6].setValue(this, new org.eyedb.Value(_attrnum), 0);
  }

  public int getAttrnum()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[6].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setOidO(org.eyedb.Oid _oid_o)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[7].setValue(this, new org.eyedb.Value(_oid_o), 0);
  }

  public org.eyedb.Oid getOidO()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[7].getValue(this, 0, true);
    return __x.sgetOid();
  }

  public void setOidN(org.eyedb.Oid _oid_n)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[8].setValue(this, new org.eyedb.Value(_oid_n), 0);
  }

  public org.eyedb.Oid getOidN()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[8].getValue(this, 0, true);
    return __x.sgetOid();
  }

  public void setRoidO(org.eyedb.Oid _roid_o)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[9].setValue(this, new org.eyedb.Value(_roid_o), 0);
  }

  public org.eyedb.Oid getRoidO()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[9].getValue(this, 0, true);
    return __x.sgetOid();
  }

  public void setSync(int _sync)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[10].setValue(this, new org.eyedb.Value(_sync), 0);
  }

  public int getSync()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[10].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setWithCheck(int _with_check)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[11].setValue(this, new org.eyedb.Value(_with_check), 0);
  }

  public int getWithCheck()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[11].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setSrcDim(int _src_dim)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[12].setValue(this, new org.eyedb.Value(_src_dim), 0);
  }

  public int getSrcDim()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[12].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setDestDim(int _dest_dim)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[13].setValue(this, new org.eyedb.Value(_dest_dim), 0);
  }

  public int getDestDim()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[13].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setOffsetO(int _offset_o)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[14].setValue(this, new org.eyedb.Value(_offset_o), 0);
  }

  public int getOffsetO()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[14].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setOffsetN(int _offset_n)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[15].setValue(this, new org.eyedb.Value(_offset_n), 0);
  }

  public int getOffsetN()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[15].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setSizeO(int _size_o)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[16].setValue(this, new org.eyedb.Value(_size_o), 0);
  }

  public int getSizeO()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[16].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setSizeN(int _size_n)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[17].setValue(this, new org.eyedb.Value(_size_n), 0);
  }

  public int getSizeN()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[17].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setCnvMth(BEMethod_C _cnv_mth)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[18].setValue(this, new org.eyedb.Value(_cnv_mth), 0);
  }

  public BEMethod_C getCnvMth()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[18].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[18].setValue(this, new org.eyedb.Value(__y), 0);
    return (BEMethod_C)__y;
  }

  public void setCnvMthOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[18].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getCnvMthOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[18].getOid(this, 0);
    return __x;
  }


  protected ClassConversion(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected ClassConversion(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected ClassConversion(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected ClassConversion(ClassConversion x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass ClassConversion_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (ClassConversion_class == null)
      return new StructClass("class_conversion", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[19];
    int[] dims;

    dims = null;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("class_update_type") : ClassUpdateType.idbclass), idbclass, "updtype", 2, false, 0, dims);

    dims = null;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("attribute_convert_type") : AttributeConvertType.idbclass), idbclass, "cnvtype", 3, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "clsname", 4, false, 1, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "attrname", 5, false, 1, dims);

    dims = null;
    attr[6] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "attrnum", 6, false, 0, dims);

    dims = null;
    attr[7] = new org.eyedb.Attribute(((m != null) ? m.getClass("oid") : OidP.idbclass), idbclass, "oid_o", 7, false, 0, dims);

    dims = null;
    attr[8] = new org.eyedb.Attribute(((m != null) ? m.getClass("oid") : OidP.idbclass), idbclass, "oid_n", 8, false, 0, dims);

    dims = null;
    attr[9] = new org.eyedb.Attribute(((m != null) ? m.getClass("oid") : OidP.idbclass), idbclass, "roid_o", 9, false, 0, dims);

    dims = null;
    attr[10] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "sync", 10, false, 0, dims);

    dims = null;
    attr[11] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "with_check", 11, false, 0, dims);

    dims = null;
    attr[12] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "src_dim", 12, false, 0, dims);

    dims = null;
    attr[13] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "dest_dim", 13, false, 0, dims);

    dims = null;
    attr[14] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "offset_o", 14, false, 0, dims);

    dims = null;
    attr[15] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "offset_n", 15, false, 0, dims);

    dims = null;
    attr[16] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "size_o", 16, false, 0, dims);

    dims = null;
    attr[17] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "size_n", 17, false, 0, dims);

    dims = null;
    attr[18] = new org.eyedb.Attribute(((m != null) ? m.getClass("be_method_C") : BEMethod_C.idbclass), idbclass, "cnv_mth", 18, true, 0, dims);

    ClassConversion_class.setAttributes(attr, 2, 17);

    return ClassConversion_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("class_conversion", ClassConversion.class.getConstructor(Database.clazz));
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

