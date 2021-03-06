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

public class Database extends org.eyedb.Database {

  public Database(String name) {super(name);}

  public Database(String name, String dbmfile) {super(name, dbmfile);}

  public Database(int dbid) {super(dbid);}

  public Database(int dbid, String dbmfile) {super(dbid, dbmfile);}

  public void open(org.eyedb.Connection conn, int flags, String userauth, String passwdauth) throws org.eyedb.Exception
  {
    super.open(conn, flags, userauth, passwdauth);

    checkSchema(getSchema());
  }

  public org.eyedb.Object loadObjectRealize(org.eyedb.Oid oid, int lockmode, org.eyedb.RecMode rcm)
  throws org.eyedb.Exception
  {
    org.eyedb.Object o = super.loadObjectRealize(oid, lockmode, rcm);
    org.eyedb.Object ro = makeObject(o, true);
    if (ro != null) o = ro;
    return o;
  }

  private void checkSchema(org.eyedb.Schema m) throws org.eyedb.Exception {
    org.eyedb.Class cl;
    String msg = "";

    if ((cl = m.getClass("index_type")) == null)
      msg += "class 'index_type' does not exist\n";
    else if (!IndexType.idbclass.compare(cl))
      msg += "class 'index_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("executable_lang")) == null)
      msg += "class 'executable_lang' does not exist\n";
    else if (!ExecutableLang.idbclass.compare(cl))
      msg += "class 'executable_lang' differs in database and in runtime environment\n";
    if ((cl = m.getClass("argtype_type")) == null)
      msg += "class 'argtype_type' does not exist\n";
    else if (!ArgType_Type.idbclass.compare(cl))
      msg += "class 'argtype_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("executable_localisation")) == null)
      msg += "class 'executable_localisation' does not exist\n";
    else if (!ExecutableLocalisation.idbclass.compare(cl))
      msg += "class 'executable_localisation' differs in database and in runtime environment\n";
    if ((cl = m.getClass("executable_type")) == null)
      msg += "class 'executable_type' does not exist\n";
    else if (!ExecutableType.idbclass.compare(cl))
      msg += "class 'executable_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("trigger_type")) == null)
      msg += "class 'trigger_type' does not exist\n";
    else if (!TriggerType.idbclass.compare(cl))
      msg += "class 'trigger_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("protection_mode")) == null)
      msg += "class 'protection_mode' does not exist\n";
    else if (!ProtectionMode.idbclass.compare(cl))
      msg += "class 'protection_mode' differs in database and in runtime environment\n";
    if ((cl = m.getClass("class_update_type")) == null)
      msg += "class 'class_update_type' does not exist\n";
    else if (!ClassUpdateType.idbclass.compare(cl))
      msg += "class 'class_update_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("attribute_convert_type")) == null)
      msg += "class 'attribute_convert_type' does not exist\n";
    else if (!AttributeConvertType.idbclass.compare(cl))
      msg += "class 'attribute_convert_type' differs in database and in runtime environment\n";
    if ((cl = m.getClass("attribute_component")) == null)
      msg += "class 'attribute_component' does not exist\n";
    else if (!AttributeComponent.idbclass.compare(cl))
      msg += "class 'attribute_component' differs in database and in runtime environment\n";
    if ((cl = m.getClass("attribute_component_set")) == null)
      msg += "class 'attribute_component_set' does not exist\n";
    else if (!AttributeComponentSet.idbclass.compare(cl))
      msg += "class 'attribute_component_set' differs in database and in runtime environment\n";
    if ((cl = m.getClass("class_component")) == null)
      msg += "class 'class_component' does not exist\n";
    else if (!ClassComponent.idbclass.compare(cl))
      msg += "class 'class_component' differs in database and in runtime environment\n";
    if ((cl = m.getClass("agregat_class_component")) == null)
      msg += "class 'agregat_class_component' does not exist\n";
    else if (!AgregatClassComponent.idbclass.compare(cl))
      msg += "class 'agregat_class_component' differs in database and in runtime environment\n";
    if ((cl = m.getClass("class_variable")) == null)
      msg += "class 'class_variable' does not exist\n";
    else if (!ClassVariable.idbclass.compare(cl))
      msg += "class 'class_variable' differs in database and in runtime environment\n";
    if ((cl = m.getClass("index")) == null)
      msg += "class 'index' does not exist\n";
    else if (!Index.idbclass.compare(cl))
      msg += "class 'index' differs in database and in runtime environment\n";
    if ((cl = m.getClass("hashindex")) == null)
      msg += "class 'hashindex' does not exist\n";
    else if (!HashIndex.idbclass.compare(cl))
      msg += "class 'hashindex' differs in database and in runtime environment\n";
    if ((cl = m.getClass("btreeindex")) == null)
      msg += "class 'btreeindex' does not exist\n";
    else if (!BTreeIndex.idbclass.compare(cl))
      msg += "class 'btreeindex' differs in database and in runtime environment\n";
    if ((cl = m.getClass("collection_attribute_implementation")) == null)
      msg += "class 'collection_attribute_implementation' does not exist\n";
    else if (!CollAttrImpl.idbclass.compare(cl))
      msg += "class 'collection_attribute_implementation' differs in database and in runtime environment\n";
    if ((cl = m.getClass("argtype")) == null)
      msg += "class 'argtype' does not exist\n";
    else if (!ArgType.idbclass.compare(cl))
      msg += "class 'argtype' differs in database and in runtime environment\n";
    if ((cl = m.getClass("signature")) == null)
      msg += "class 'signature' does not exist\n";
    else if (!Signature.idbclass.compare(cl))
      msg += "class 'signature' differs in database and in runtime environment\n";
    if ((cl = m.getClass("executable")) == null)
      msg += "class 'executable' does not exist\n";
    else if (!Executable.idbclass.compare(cl))
      msg += "class 'executable' differs in database and in runtime environment\n";
    if ((cl = m.getClass("agregat_class_executable")) == null)
      msg += "class 'agregat_class_executable' does not exist\n";
    else if (!AgregatClassExecutable.idbclass.compare(cl))
      msg += "class 'agregat_class_executable' differs in database and in runtime environment\n";
    if ((cl = m.getClass("method")) == null)
      msg += "class 'method' does not exist\n";
    else if (!Method.idbclass.compare(cl))
      msg += "class 'method' differs in database and in runtime environment\n";
    if ((cl = m.getClass("fe_method")) == null)
      msg += "class 'fe_method' does not exist\n";
    else if (!FEMethod.idbclass.compare(cl))
      msg += "class 'fe_method' differs in database and in runtime environment\n";
    if ((cl = m.getClass("fe_method_C")) == null)
      msg += "class 'fe_method_C' does not exist\n";
    else if (!FEMethod_C.idbclass.compare(cl))
      msg += "class 'fe_method_C' differs in database and in runtime environment\n";
    if ((cl = m.getClass("be_method")) == null)
      msg += "class 'be_method' does not exist\n";
    else if (!BEMethod.idbclass.compare(cl))
      msg += "class 'be_method' differs in database and in runtime environment\n";
    if ((cl = m.getClass("be_method_C")) == null)
      msg += "class 'be_method_C' does not exist\n";
    else if (!BEMethod_C.idbclass.compare(cl))
      msg += "class 'be_method_C' differs in database and in runtime environment\n";
    if ((cl = m.getClass("be_method_OQL")) == null)
      msg += "class 'be_method_OQL' does not exist\n";
    else if (!BEMethod_OQL.idbclass.compare(cl))
      msg += "class 'be_method_OQL' differs in database and in runtime environment\n";
    if ((cl = m.getClass("trigger")) == null)
      msg += "class 'trigger' does not exist\n";
    else if (!Trigger.idbclass.compare(cl))
      msg += "class 'trigger' differs in database and in runtime environment\n";
    if ((cl = m.getClass("unique_constraint")) == null)
      msg += "class 'unique_constraint' does not exist\n";
    else if (!UniqueConstraint.idbclass.compare(cl))
      msg += "class 'unique_constraint' differs in database and in runtime environment\n";
    if ((cl = m.getClass("notnull_constraint")) == null)
      msg += "class 'notnull_constraint' does not exist\n";
    else if (!NotNullConstraint.idbclass.compare(cl))
      msg += "class 'notnull_constraint' differs in database and in runtime environment\n";
    if ((cl = m.getClass("cardinality_description")) == null)
      msg += "class 'cardinality_description' does not exist\n";
    else if (!CardinalityDescription.idbclass.compare(cl))
      msg += "class 'cardinality_description' differs in database and in runtime environment\n";
    if ((cl = m.getClass("cardinality_constraint")) == null)
      msg += "class 'cardinality_constraint' does not exist\n";
    else if (!CardinalityConstraint.idbclass.compare(cl))
      msg += "class 'cardinality_constraint' differs in database and in runtime environment\n";
    if ((cl = m.getClass("cardinality_constraint_test")) == null)
      msg += "class 'cardinality_constraint_test' does not exist\n";
    else if (!CardinalityConstraint_Test.idbclass.compare(cl))
      msg += "class 'cardinality_constraint_test' differs in database and in runtime environment\n";
    if ((cl = m.getClass("protection_user")) == null)
      msg += "class 'protection_user' does not exist\n";
    else if (!ProtectionUser.idbclass.compare(cl))
      msg += "class 'protection_user' differs in database and in runtime environment\n";
    if ((cl = m.getClass("protection")) == null)
      msg += "class 'protection' does not exist\n";
    else if (!Protection.idbclass.compare(cl))
      msg += "class 'protection' differs in database and in runtime environment\n";
    if ((cl = m.getClass("unreadable_object")) == null)
      msg += "class 'unreadable_object' does not exist\n";
    else if (!UnreadableObject.idbclass.compare(cl))
      msg += "class 'unreadable_object' differs in database and in runtime environment\n";
    if ((cl = m.getClass("class_conversion")) == null)
      msg += "class 'class_conversion' does not exist\n";
    else if (!ClassConversion.idbclass.compare(cl))
      msg += "class 'class_conversion' differs in database and in runtime environment\n";
    if (!msg.equals("")) throw new org.eyedb.Exception(new org.eyedb.Status(org.eyedb.Status.IDB_ERROR, msg));
  }

  static public org.eyedb.Object makeObject(org.eyedb.Object o, boolean share)
  throws org.eyedb.Exception {

    if (o == null || o.getClass(true) == null) return o;

    if (o.isGRTObject()) return o;

    try {
      java.lang.reflect.Constructor cons = (java.lang.reflect.Constructor)hash.get(o.getClass(true).getName());
      if (cons == null) return o;

      java.lang.Object[] tmp = new java.lang.Object[2]; tmp[0] = o; tmp[1] = new java.lang.Boolean(share);
      return (org.eyedb.Object)cons.newInstance(tmp);
    } catch(java.lang.Exception e) {
      System.err.println("caught " + e + " in database");
      System.exit(2);
      return null;
    }
  }

  static java.util.Hashtable hash = new java.util.Hashtable(256);
  static protected java.lang.Class[] clazz;
  static {
    clazz = new java.lang.Class[2];
    clazz[0] = org.eyedb.Struct.class;
    clazz[1] = boolean.class;
  }

  public static void init()
 throws org.eyedb.Exception {
    IndexType.init_p();
    ExecutableLang.init_p();
    ArgType_Type.init_p();
    ExecutableLocalisation.init_p();
    ExecutableType.init_p();
    TriggerType.init_p();
    ProtectionMode.init_p();
    ClassUpdateType.init_p();
    AttributeConvertType.init_p();
    Root.init_p();
    AttributeComponent.init_p();
    AttributeComponentSet.init_p();
    ClassComponent.init_p();
    AgregatClassComponent.init_p();
    ClassVariable.init_p();
    Index.init_p();
    HashIndex.init_p();
    BTreeIndex.init_p();
    CollAttrImpl.init_p();
    ArgType.init_p();
    Signature.init_p();
    Executable.init_p();
    AgregatClassExecutable.init_p();
    Method.init_p();
    FEMethod.init_p();
    FEMethod_C.init_p();
    BEMethod.init_p();
    BEMethod_C.init_p();
    BEMethod_OQL.init_p();
    Trigger.init_p();
    UniqueConstraint.init_p();
    NotNullConstraint.init_p();
    CardinalityDescription.init_p();
    CardinalityConstraint.init_p();
    CardinalityConstraint_Test.init_p();
    ProtectionUser.init_p();
    Protection.init_p();
    UnreadableObject.init_p();
    ClassConversion.init_p();
    set_class_AttributeComponent_ref.init_p();
    IndexType.init();
    ExecutableLang.init();
    ArgType_Type.init();
    ExecutableLocalisation.init();
    ExecutableType.init();
    TriggerType.init();
    ProtectionMode.init();
    ClassUpdateType.init();
    AttributeConvertType.init();
    Root.init();
    AttributeComponent.init();
    AttributeComponentSet.init();
    ClassComponent.init();
    AgregatClassComponent.init();
    ClassVariable.init();
    Index.init();
    HashIndex.init();
    BTreeIndex.init();
    CollAttrImpl.init();
    ArgType.init();
    Signature.init();
    Executable.init();
    AgregatClassExecutable.init();
    Method.init();
    FEMethod.init();
    FEMethod_C.init();
    BEMethod.init();
    BEMethod_C.init();
    BEMethod_OQL.init();
    Trigger.init();
    UniqueConstraint.init();
    NotNullConstraint.init();
    CardinalityDescription.init();
    CardinalityConstraint.init();
    CardinalityConstraint_Test.init();
    ProtectionUser.init();
    Protection.init();
    UnreadableObject.init();
    ClassConversion.init();
  }
}

