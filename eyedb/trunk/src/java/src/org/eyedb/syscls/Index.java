
//
// class Index
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Mon May 28 11:17:02 2007
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class Index extends AttributeComponent {

  public Index(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public Index(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("index") : Index.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Index(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Index(Index x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setIdxOid(org.eyedb.Oid _idx_oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[6].setValue(this, new org.eyedb.Value(_idx_oid), 0);
  }

  public org.eyedb.Oid getIdxOid()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[6].getValue(this, 0, true);
    return __x.sgetOid();
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

  public void setIsString(int _is_string)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[8].setValue(this, new org.eyedb.Value(_is_string), 0);
  }

  public int getIsString()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[8].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setImplHints(int a0, int _impl_hints)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[9].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[9].setSize(this, from+1);
    getClass(true).getAttributes()[9].setValue(this, new org.eyedb.Value(_impl_hints), from);
  }

  public int getImplHints(int a0)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;
    int from = a0;

    __x = getClass(true).getAttributes()[9].getValue(this, from, true);
    return __x.sgetInt();
  }

  int getImpl_hints_cnt()
  {
    return getClass(true).getAttributes()[9].getSize(this);
  }


  protected Index(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected Index(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected Index(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected Index(Index x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass Index_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (Index_class == null)
      return new org.eyedb.StructClass("index", ((m != null) ? m.getClass("eyedb::AttributeComponent") : AttributeComponent.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[10];
    int[] dims;

    dims = null;
    attr[6] = new org.eyedb.Attribute(((m != null) ? m.getClass("oid") : org.eyedb.OidP.idbclass), idbclass, "idx_oid", 6, false, 0, dims);

    dims = null;
    attr[7] = new org.eyedb.Attribute(((m != null) ? m.getClass("short") : org.eyedb.Int16.idbclass), idbclass, "dspid", 7, false, 0, dims);

    dims = null;
    attr[8] = new org.eyedb.Attribute(((m != null) ? m.getClass("bool") : org.eyedb.Bool.idbclass), idbclass, "is_string", 8, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[9] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : org.eyedb.Int32.idbclass), idbclass, "impl_hints", 9, false, 1, dims);

    Index_class.setAttributes(attr, 6, 4);

    return Index_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("index", Index.class.getConstructor(Database.clazz));
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

