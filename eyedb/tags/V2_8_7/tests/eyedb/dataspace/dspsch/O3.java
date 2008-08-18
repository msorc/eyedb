
//
// class O3
//
// package dspsch
//
// Generated by eyedbodl at Wed Jan 18 00:11:48 2006
//

package dspsch;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class O3 extends org.eyedb.Struct {

  public O3(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db);
    initialize(db);
  }

  public O3(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("O3") : O3.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public O3(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public O3(O3 x, boolean share) throws org.eyedb.Exception {
    super(x, share);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setN3(int _n3)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_n3), 0);
  }

  public int getN3()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[2].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setData3(String _data3)
  throws org.eyedb.Exception {
    int len = _data3.length() + 1;

    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[3].setSize(this, len);
    getClass(true).getAttributes()[3].setStringValue(this, _data3);
  }

  public void setData3(int a0, char _data3)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[3].setSize(this, from+1);
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_data3), from);
  }

  public String getData3()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[3].getStringValue(this);
  }


  protected O3(org.eyedb.Database db, int dummy) {
    super(db);
  }

  protected O3(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace);
  }

  protected O3(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share);
  }

  protected O3(O3 x, boolean share, int dummy) {
     super(x, share);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass O3_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (O3_class == null)
      return new org.eyedb.StructClass("O3", ((m != null) ? m.getClass("org.eyedb.Struct") : org.eyedb.Struct.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[4];
    int[] dims;

    dims = null;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : org.eyedb.Int32.idbclass), idbclass, "org.eyedb.Int32", 2, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "org.eyedb.Char", 3, false, 1, dims);

    O3_class.setAttributes(attr, 2, 2);

    return O3_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("O3", O3.class.getConstructor(Database.clazz));
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

