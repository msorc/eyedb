
//
// class Method
//
// package syscls
//
// Generated by eyedbodl at Fri Feb  9 14:16:44 2007
//

package syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class Method extends AgregatClassExecutable {

  public Method(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public Method(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("method") : Method.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Method(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Method(Method x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }


  protected Method(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected Method(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected Method(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected Method(Method x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass Method_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (Method_class == null)
      return new org.eyedb.StructClass("method", ((m != null) ? m.getClass("eyedb::AgregatClassExecutable") : AgregatClassExecutable.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[5];
    int[] dims;

    Method_class.setAttributes(attr, 5, 0);

    return Method_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("method", Method.class.getConstructor(Database.clazz));
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

