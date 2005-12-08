/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/

package org.eyedb.syscls;

import org.eyedb.*;
import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class AttributeComponentSet extends Root {

  public AttributeComponentSet(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public AttributeComponentSet(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("attribute_component_set") : AttributeComponentSet.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public AttributeComponentSet(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public AttributeComponentSet(AttributeComponentSet x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public void setAttrname(String _attrname)
  throws org.eyedb.Exception {
    int len = _attrname.length() + 1;

    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[2].setSize(this, len);
    getClass(true).getAttributes()[2].setStringValue(this, _attrname);
  }

  public void setAttrname(int a0, char _attrname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[2].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[2].setSize(this, from+1);
    getClass(true).getAttributes()[2].setValue(this, new org.eyedb.Value(_attrname), from);
  }

  public String getAttrname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[2].getStringValue(this);
  }

  public int getCompsCount() throws org.eyedb.Exception {
    org.eyedb.Collection _coll = getCompsColl();
    return (_coll != null ? _coll.getCount() : 0);
  }

  public void setCompsColl(org.eyedb.CollSet _comps)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_comps), 0);
  }

  public org.eyedb.CollSet getCompsColl()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[3].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(__y), 0);
    return (org.eyedb.CollSet)__y;
  }

  public AttributeComponent getCompsAt(int ind)
  throws org.eyedb.Exception {
    org.eyedb.Collection coll = getCompsColl();

    if (coll == null)
      return null;

    AttributeComponent tmp;
    tmp = (AttributeComponent)coll.getObjectAt(ind);
    return tmp;
  }

  public void addToCompsColl(AttributeComponent _comps)
  throws org.eyedb.Exception {
    addToCompsColl(_comps, false);
  }

  public void addToCompsColl(AttributeComponent _comps, boolean noDup)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.CollSet _coll;
    boolean _not_set = false;
    int from = 0;

    __x = getClass(true).getAttributes()[3].getValue(this, from, true);
    _coll = (org.eyedb.CollSet)__x.sgetObject();
    if (_coll == null) {
        _coll = new org.eyedb.CollSet(db, "", db.getSchema().getClass("attribute_component"), true);
        _not_set = true;
    }

    _coll.insert(_comps, noDup);
    if (!_not_set)
      return;
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_coll), from);
  }

  public void addToCompsColl(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    addToCompsColl(_oid, false);
  }

  public void addToCompsColl(org.eyedb.Oid _oid, boolean noDup)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.CollSet _coll;
    boolean _not_set = false;
    int from = 0;

    __x = getClass(true).getAttributes()[3].getValue(this, from, true);
    _coll = (org.eyedb.CollSet)__x.sgetObject();
    if (_coll == null) {
        _coll = new org.eyedb.CollSet(db, "", db.getSchema().getClass("attribute_component"), true);
        _not_set = true;
    }

    _coll.insert(_oid, noDup);
    if (!_not_set)
      return;
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_coll), from);
  }

  public void rmvFromCompsColl(AttributeComponent _comps)
  throws org.eyedb.Exception {
    rmvFromCompsColl(_comps, false);
  }

  public void rmvFromCompsColl(AttributeComponent _comps, boolean checkFirst)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.CollSet _coll;
    boolean _not_set = false;
    int from = 0;

    __x = getClass(true).getAttributes()[3].getValue(this, from, true);
    _coll = (org.eyedb.CollSet)__x.sgetObject();
    if (_coll == null)
        throw new org.eyedb.Exception(org.eyedb.Status.IDB_ERROR, "invalid collection", "no valid collection in attribute AttributeComponentSet::comps");


    _coll.suppress(_comps, checkFirst);
    if (!_not_set)
      return;
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_coll), from);
  }

  public void rmvFromCompsColl(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    rmvFromCompsColl(_oid, false);
  }

  public void rmvFromCompsColl(org.eyedb.Oid _oid, boolean checkFirst)
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.CollSet _coll;
    boolean _not_set = false;
    int from = 0;

    __x = getClass(true).getAttributes()[3].getValue(this, from, true);
    _coll = (org.eyedb.CollSet)__x.sgetObject();
    if (_coll == null)
        throw new org.eyedb.Exception(org.eyedb.Status.IDB_ERROR, "invalid collection", "no valid collection in attribute AttributeComponentSet::comps");


    _coll.suppress(_oid, checkFirst);
    if (!_not_set)
      return;
    getClass(true).getAttributes()[3].setValue(this, new org.eyedb.Value(_coll), from);
  }

  public void setClassOwner(org.eyedb.Class _class_owner)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_class_owner), 0);
  }

  public org.eyedb.Class getClassOwner()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[4].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(__y), 0);
    return (org.eyedb.Class)__y;
  }

  public void setClassOwnerOid_oid(org.eyedb.Oid _oid)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[4].setOid(this, _oid, 0);
  }

  public org.eyedb.Oid getClassOwnerOid_oid()
  throws org.eyedb.Exception {
    org.eyedb.Oid __x;

    __x = getClass(true).getAttributes()[4].getOid(this, 0);
    return __x;
  }


  protected AttributeComponentSet(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected AttributeComponentSet(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected AttributeComponentSet(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected AttributeComponentSet(AttributeComponentSet x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static StructClass make(StructClass AttributeComponentSet_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (AttributeComponentSet_class == null)
      return new StructClass("attribute_component_set", ((m != null) ? m.getClass("Root") : Root.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[5];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[2] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "attrname", 2, false, 1, dims);

    dims = null;
    attr[3] = new org.eyedb.Attribute(((m != null) ? m.getClass("set<attribute_component*>") : set_class_AttributeComponent_ref.idbclass), idbclass, "comps", 3, false, 0, dims);

    dims = null;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("class") : org.eyedb.Class.idbclass), idbclass, "class_owner", 4, true, 0, dims);

    AttributeComponentSet_class.setAttributes(attr, 2, 3);

    return AttributeComponentSet_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("attribute_component_set", AttributeComponentSet.class.getConstructor(Database.clazz));
    } catch(java.lang.Exception e) {
      e.printStackTrace();
    }
  }

  static void init()
   throws org.eyedb.Exception {
    make((StructClass)idbclass, null);

    idr_objsz = idbclass.getObjectSize();
    idr_psize = idbclass.getObjectPSize();
  }

}

