
//
// class HashIndex
//
// package syscls
//
// Generated by eyedbodl at Fri Feb  9 14:16:44 2007
//

package syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class HashIndex extends Index {

  public HashIndex(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public HashIndex(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("hashindex") : HashIndex.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public HashIndex(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public HashIndex(HashIndex x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setHashMethod(BEMethod_C _hash_method)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[10].setValue(this, new org.eyedb.Value(_hash_method), 0);
  }

  public BEMethod_C getHashMethod()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[10].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[10].setValue(this, new org.eyedb.Value(__y), 0);
    return (BEMethod_C)__y;
  }

  public void setHashMethodOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[10].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getHashMethodOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[10].getOid(this, 0);
    return __x;
  }

  public void setKeyCount(int _key_count)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[11].setValue(this, new org.eyedb.Value(_key_count), 0);
  }

  public int getKeyCount()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[11].getValue(this, 0, true);
    return __x.sgetInt();
  }


  protected HashIndex(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected HashIndex(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected HashIndex(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected HashIndex(HashIndex x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass HashIndex_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (HashIndex_class == null)
      return new org.eyedb.StructClass("hashindex", ((m != null) ? m.getClass("eyedb::Index") : Index.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[12];
    int[] dims;

    dims = null;
    attr[10] = new org.eyedb.Attribute(((m != null) ? m.getClass("be_method_C") : BEMethod_C.idbclass), idbclass, "hash_method", 10, true, 0, dims);

    dims = null;
    attr[11] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : org.eyedb.Int32.idbclass), idbclass, "key_count", 11, false, 0, dims);

    HashIndex_class.setAttributes(attr, 10, 2);

    return HashIndex_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("hashindex", HashIndex.class.getConstructor(Database.clazz));
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

