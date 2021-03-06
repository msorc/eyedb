
//
// class UserEntry
//
// package org.eyedb.dbm
//
// Generated by eyedbodl at Fri Jan 18 22:55:16 2008
//

package org.eyedb.dbm;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class UserEntry extends Root {

  public UserEntry(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public UserEntry(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("user_entry") : UserEntry.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public UserEntry(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public UserEntry(UserEntry x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setName(String _name)
  throws org.eyedb.Exception {
    int len = _name.length() + 1;

    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[2].setSize(this, len);
    getClass(true).getAttributes()[2].setStringValue(this, _name);
  }

  public void setName(int a0, char _name)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[2].setSize(this, from+1);
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_name), from);
  }

  public String getName()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[2].getStringValue(this);
  }

  public void setPasswd(String _passwd)
  throws org.eyedb.Exception {
    int len = _passwd.length() + 1;

    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[3].setSize(this, len);
    getClass(true).getAttributes()[3].setStringValue(this, _passwd);
  }

  public void setPasswd(int a0, char _passwd)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[3].setSize(this, from+1);
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_passwd), from);
  }

  public String getPasswd()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[3].getStringValue(this);
  }

  public void setUid(int _uid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_uid), 0);
  }

  public int getUid()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[4].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setType(int _type)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_type), 0);
  }

  public int getType()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[5].getValue(this, 0, true);
    return __x.sgetInt();
  }


  protected UserEntry(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected UserEntry(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected UserEntry(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected UserEntry(UserEntry x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass UserEntry_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (UserEntry_class == null)
      return new org.eyedb.StructClass("user_entry", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[6];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "name", 2, false, 1, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "passwd", 3, false, 1, dims);

    dims = null;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("int") : org.eyedb.Int32.idbclass), idbclass, "uid", 4, false, 0, dims);

    dims = null;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("user_type") : UserType.idbclass), idbclass, "type", 5, false, 0, dims);

    UserEntry_class.setAttributes(attr, 2, 4);

    return UserEntry_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("user_entry", UserEntry.class.getConstructor(Database.clazz));
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

