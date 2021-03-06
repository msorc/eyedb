
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

package dspsch;

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

    if ((cl = m.getClass("O0")) == null)
      msg += "class 'O0' does not exist\n";
    else if (!O0.idbclass.compare(cl))
      msg += "class 'O0' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O1")) == null)
      msg += "class 'O1' does not exist\n";
    else if (!O1.idbclass.compare(cl))
      msg += "class 'O1' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O2")) == null)
      msg += "class 'O2' does not exist\n";
    else if (!O2.idbclass.compare(cl))
      msg += "class 'O2' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O3")) == null)
      msg += "class 'O3' does not exist\n";
    else if (!O3.idbclass.compare(cl))
      msg += "class 'O3' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O4")) == null)
      msg += "class 'O4' does not exist\n";
    else if (!O4.idbclass.compare(cl))
      msg += "class 'O4' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O5")) == null)
      msg += "class 'O5' does not exist\n";
    else if (!O5.idbclass.compare(cl))
      msg += "class 'O5' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O1_1")) == null)
      msg += "class 'O1_1' does not exist\n";
    else if (!O1_1.idbclass.compare(cl))
      msg += "class 'O1_1' differs in database and in runtime environment\n";
    if ((cl = m.getClass("O1_1_1")) == null)
      msg += "class 'O1_1_1' does not exist\n";
    else if (!O1_1_1.idbclass.compare(cl))
      msg += "class 'O1_1_1' differs in database and in runtime environment\n";
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
    O0.init_p();
    O1.init_p();
    O2.init_p();
    O3.init_p();
    O4.init_p();
    O5.init_p();
    O1_1.init_p();
    O1_1_1.init_p();
    O0.init();
    O1.init();
    O2.init();
    O3.init();
    O4.init();
    O5.init();
    O1_1.init();
    O1_1_1.init();
  }
}

