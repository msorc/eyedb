
//
// class BTreeIndex
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Sun Feb 11 18:15:55 2007
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class BTreeIndex extends Index {

  public BTreeIndex(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public BTreeIndex(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("btreeindex") : BTreeIndex.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public BTreeIndex(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public BTreeIndex(BTreeIndex x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setDegree(int _degree)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[10].setValue(this, new org.eyedb.Value(_degree), 0);
  }

  public int getDegree()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[10].getValue(this, 0, true);
    return __x.sgetInt();
  }


  protected BTreeIndex(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected BTreeIndex(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected BTreeIndex(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected BTreeIndex(BTreeIndex x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass BTreeIndex_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (BTreeIndex_class == null)
      return new org.eyedb.StructClass("btreeindex", ((m != null) ? m.getClass("eyedb::Index") : Index.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[11];
    int[] dims;

    dims = null;
    attr[10] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : org.eyedb.Int32.idbclass), idbclass, "degree", 10, false, 0, dims);

    BTreeIndex_class.setAttributes(attr, 10, 1);

    return BTreeIndex_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("btreeindex", BTreeIndex.class.getConstructor(Database.clazz));
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

