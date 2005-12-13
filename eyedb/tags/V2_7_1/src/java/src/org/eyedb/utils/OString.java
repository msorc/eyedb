
//
// class OString
//
// package org.eyedb.utils
//
// Generated by eyedbodl at Sat Dec 10 16:57:25 2005
//

package org.eyedb.utils;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class OString extends Root {

  public OString(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public OString(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("ostring") : OString.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public OString(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public OString(OString x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setS(String _s)
  throws org.eyedb.Exception {
    int len = _s.length() + 1;

    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[2].setSize(this, len);
    getClass(true).getAttributes()[2].setStringValue(this, _s);
  }

  public void setS(int a0, char _s)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[2].setSize(this, from+1);
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_s), from);
  }

  public String getS()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[2].getStringValue(this);
  }


  protected OString(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected OString(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected OString(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected OString(OString x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass OString_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (OString_class == null)
      return new org.eyedb.StructClass("ostring", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[3];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "org.eyedb.Char", 2, false, 1, dims);

    OString_class.setAttributes(attr, 2, 1);

    return OString_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("ostring", OString.class.getConstructor(Database.clazz));
    } catch(java.lang.Exception e) {
      e.printStackTrace();
    }
  }

  static void init()
   throws org.eyedb.Exception {
    make((org.eyedb.StructClass)idbclass, null);

    idr_objsz = idbclass.getObjectSize();
    idr_psize = idbclass.getObjectPSize();
  }

}

