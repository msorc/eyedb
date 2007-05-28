
//
// class CURL
//
// package org.eyedb.utils
//
// Generated by eyedbodl at Mon May 28 11:17:03 2007
//

package org.eyedb.utils;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class CURL extends Root {

  public CURL(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public CURL(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("URL") : org.eyedb.utils.CURL.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CURL(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CURL(CURL x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setProto(String _proto)
  throws org.eyedb.Exception {
    int len = _proto.length() + 1;

    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[2].setSize(this, len);
    getClass(true).getAttributes()[2].setStringValue(this, _proto);
  }

  public void setProto(int a0, char _proto)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[2].setSize(this, from+1);
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_proto), from);
  }

  public String getProto()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[2].getStringValue(this);
  }

  public void setUrl(String _url)
  throws org.eyedb.Exception {
    int len = _url.length() + 1;

    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[3].setSize(this, len);
    getClass(true).getAttributes()[3].setStringValue(this, _url);
  }

  public void setUrl(int a0, char _url)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[3].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[3].setSize(this, from+1);
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_url), from);
  }

  public String getUrl()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[3].getStringValue(this);
  }


  protected CURL(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected CURL(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected CURL(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected CURL(CURL x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass CURL_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (CURL_class == null)
      return new org.eyedb.StructClass("URL", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[4];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "proto", 2, false, 1, dims);

    dims = new int[1];
    dims[0] = -1;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : org.eyedb.Char.idbclass), idbclass, "url", 3, false, 1, dims);

    CURL_class.setAttributes(attr, 2, 2);

    return CURL_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("URL", CURL.class.getConstructor(Database.clazz));
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

