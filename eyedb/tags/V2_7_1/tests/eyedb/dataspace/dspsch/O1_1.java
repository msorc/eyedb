
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

package dspsch;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class O1_1 extends O1 {

  public O1_1(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public O1_1(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("O1_1") : O1_1.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public O1_1(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public O1_1(O1_1 x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setN11(int _n1_1)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_n1_1), 0);
  }

  public int getN11()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[4].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setData1_1(String _data1_1)
  throws org.eyedb.Exception {
    int len = _data1_1.length() + 1;

    int size = getClass(true).getAttributes()[5].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[5].setSize(this, len);
    getClass(true).getAttributes()[5].setStringValue(this, _data1_1);
  }

  public void setData11(int a0, char _data1_1)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[5].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[5].setSize(this, from+1);
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_data1_1), from);
  }

  public String getData1_1()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[5].getStringValue(this);
  }


  protected O1_1(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected O1_1(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected O1_1(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected O1_1(O1_1 x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass O1_1_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (O1_1_class == null)
      return new StructClass("O1_1", ((m != null) ? m.getClass("O1") : O1.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[6];
    int[] dims;

    dims = null;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : Int32.idbclass), idbclass, "n1_1", 4, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "data1_1", 5, false, 1, dims);

    O1_1_class.setAttributes(attr, 4, 2);

    return O1_1_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("O1_1", O1_1.class.getConstructor(Database.clazz));
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

