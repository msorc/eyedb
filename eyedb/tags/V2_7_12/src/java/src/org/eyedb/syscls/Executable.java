
//
// class Executable
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Sun Feb 11 18:15:55 2007
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class Executable extends Root {

  public Executable(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public Executable(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("executable") : Executable.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Executable(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public Executable(Executable x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setExname(String _exname)
  throws org.eyedb.Exception {
    int len = _exname.length() + 1;

    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[2].setSize(this, len);
    getClass(true).getAttributes()[2].setStringValue(this, _exname);
  }

  public void setExname(int a0, char _exname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[2].setSize(this, from+1);
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_exname), from);
  }

  public String getExname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[2].getStringValue(this);
  }

  public void setLang(int _lang)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_lang), 0);
  }

  public int getLang()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[3].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setLoc(int _loc)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_loc), 0);
  }

  public int getLoc()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[4].getValue(this, 0, true);
    return __x.sgetInt();
  }

  public void setSign(Signature _sign)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_sign), 0);
  }

  public Signature getSign()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[5].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(__y), 0);
    return (Signature)__y;
  }

  public void setIntname(String _intname)
  throws org.eyedb.Exception {
    int len = _intname.length() + 1;

    int size = getClass(true).getAttributes()[6].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[6].setSize(this, len);
    getClass(true).getAttributes()[6].setStringValue(this, _intname);
  }

  public void setIntname(int a0, char _intname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[6].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[6].setSize(this, from+1);
    getClass(true).getAttributes()[6].setValue(this, new org.eyedb.Value(_intname), from);
  }

  public String getIntname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[6].getStringValue(this);
  }

  public void setExtref_body(String _extref_body)
  throws org.eyedb.Exception {
    int len = _extref_body.length() + 1;

    int size = getClass(true).getAttributes()[7].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[7].setSize(this, len);
    getClass(true).getAttributes()[7].setStringValue(this, _extref_body);
  }

  public void setExtrefBody(int a0, char _extref_body)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[7].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[7].setSize(this, from+1);
    getClass(true).getAttributes()[7].setValue(this, new org.eyedb.Value(_extref_body), from);
  }

  public String getExtref_body()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[7].getStringValue(this);
  }


  protected Executable(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected Executable(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected Executable(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected Executable(Executable x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass Executable_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (Executable_class == null)
      return new org.eyedb.StructClass("executable", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[8];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "exname", 2, false, 1, dims);

    dims = null;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("executable_lang") : ExecutableLang.idbclass), idbclass, "lang", 3, false, 0, dims);

    dims = null;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("executable_localisation") : ExecutableLocalisation.idbclass), idbclass, "loc", 4, false, 0, dims);

    dims = null;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("signature") : Signature.idbclass), idbclass, "sign", 5, false, 0, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[6] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "intname", 6, false, 1, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[7] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "extref_body", 7, false, 1, dims);

    Executable_class.setAttributes(attr, 2, 6);

    return Executable_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("executable", Executable.class.getConstructor(Database.clazz));
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

