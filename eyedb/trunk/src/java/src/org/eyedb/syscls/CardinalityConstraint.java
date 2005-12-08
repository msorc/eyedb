
//
// class CardinalityConstraint
//
// package org.eyedb.syscls
//
// Generated by eyedbodl at Thu Dec  8 18:45:36 2005
//

package org.eyedb.syscls;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class CardinalityConstraint extends AgregatClassComponent {

  public CardinalityConstraint(org.eyedb.Database db) throws org.eyedb.Exception {
    super(db, 1);
    initialize(db);
  }

  public CardinalityConstraint(org.eyedb.Database db, org.eyedb.Dataspace dataspace) throws org.eyedb.Exception {
    super(db, dataspace, 1);
    initialize(db);
  }

  private void initialize(org.eyedb.Database db) throws org.eyedb.Exception {
    setClass(((db != null) ? db.getSchema().getClass("cardinality_constraint") : CardinalityConstraint.idbclass));

    setIDR(new byte[idr_objsz]);

    org.eyedb.Coder.memzero(getIDR(), org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE, idr_objsz - org.eyedb.ObjectHeader.IDB_OBJ_HEAD_SIZE);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CardinalityConstraint(org.eyedb.Struct x, boolean share) throws org.eyedb.Exception {
    super(x, share, 1);
    headerCode(org.eyedb.ObjectHeader._Struct_Type, idr_psize, org.eyedb.ObjectHeader.IDB_XINFO_LOCAL_OBJ, true);
    if (!share)
      getClass(true).newObjRealize(getDatabase(), this);
    setGRTObject(true);
    userInitialize();
  }

  public CardinalityConstraint(CardinalityConstraint x, boolean share) throws org.eyedb.Exception {
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

    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size < len)
      getClass(true).getAttributes()[4].setSize(this, len);
    getClass(true).getAttributes()[4].setStringValue(this, _attrname);
  }

  public void setAttrname(int a0, char _attrname)
  throws org.eyedb.Exception {
    int from = a0;
    int size = getClass(true).getAttributes()[4].getSize(this);
    if (size <= from)
      getClass(true).getAttributes()[4].setSize(this, from+1);
    getClass(true).getAttributes()[4].setValue(this, new org.eyedb.Value(_attrname), from);
  }

  public String getAttrname()
  throws org.eyedb.Exception {
    return getClass(true).getAttributes()[4].getStringValue(this);
  }

  public void setCardDesc(CardinalityDescription _card_desc)
  throws org.eyedb.Exception {
    getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(_card_desc), 0);
  }

  public CardinalityDescription getCardDesc()
  throws org.eyedb.Exception {
    org.eyedb.Value __x;
    org.eyedb.Object __y;

    __x = getClass(true).getAttributes()[5].getValue(this, 0, true);
    __y = Database.makeObject(__x.sgetObject(), true);
    if (__y != __x.sgetObject())
      getClass(true).getAttributes()[5].setValue(this, new org.eyedb.Value(__y), 0);
    return (CardinalityDescription)__y;
  }


  protected CardinalityConstraint(org.eyedb.Database db, int dummy) {
    super(db, 1);
  }

  protected CardinalityConstraint(org.eyedb.Database db, org.eyedb.Dataspace dataspace, int dummy) {
    super(db, dataspace, 1);
  }

  protected CardinalityConstraint(org.eyedb.Struct x, boolean share, int dummy) {
     super(x, share, 1);
  }

  protected CardinalityConstraint(CardinalityConstraint x, boolean share, int dummy) {
     super(x, share, 1);
  }

  static int idr_psize;
  static int idr_objsz;
  public static org.eyedb.Class idbclass;
  static org.eyedb.StructClass make(org.eyedb.StructClass CardinalityConstraint_class, org.eyedb.Schema m)
   throws org.eyedb.Exception {
    if (CardinalityConstraint_class == null)
      return new org.eyedb.StructClass("cardinality_constraint", ((m != null) ? m.getClass("AgregatClassComponent") : AgregatClassComponent.idbclass));
    org.eyedb.Attribute[] attr = new org.eyedb.Attribute[6];
    int[] dims;

    dims = new int[1];
    dims[0] = -1;
    attr[4] = new org.eyedb.Attribute(((m != null) ? m.getClass("char") : Char.idbclass), idbclass, "attrname", 4, false, 1, dims);

    dims = null;
    attr[5] = new org.eyedb.Attribute(((m != null) ? m.getClass("cardinality_description") : CardinalityDescription.idbclass), idbclass, "card_desc", 5, false, 0, dims);

    CardinalityConstraint_class.setAttributes(attr, 4, 2);

    return CardinalityConstraint_class;
  }

  static void init_p()
   throws org.eyedb.Exception {
    idbclass = make(null, null);
    try {
      Database.hash.put("cardinality_constraint", CardinalityConstraint.class.getConstructor(Database.clazz));
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

