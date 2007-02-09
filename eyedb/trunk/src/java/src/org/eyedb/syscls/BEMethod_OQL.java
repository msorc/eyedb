
//
// class BEMethod_OQL
//
// package syscls
//
// Generated by eyedbodl at Fri Feb  9 14:16:44 2007
//

package syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class BEMethod_OQL extends BEMethod {

  public BEMethod_OQL(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public BEMethod_OQL(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("be_method_OQL") : BEMethod_OQL.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public BEMethod_OQL(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public BEMethod_OQL(BEMethod_OQL x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }


  protected BEMethod_OQL(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected BEMethod_OQL(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected BEMethod_OQL(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected BEMethod_OQL(BEMethod_OQL x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass BEMethod_OQL_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (BEMethod_OQL_class == null)
      return new org.eyedb.StructClass("be_method_OQL", ((m != null) ? m.getClass("eyedb::BEMethod") : BEMethod.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[5];
    int[] dims;

    BEMethod_OQL_class.setAttributes(attr, 5, 0);

    return BEMethod_OQL_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("be_method_OQL", BEMethod_OQL.class.getConstructor(Database.clazz));
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

