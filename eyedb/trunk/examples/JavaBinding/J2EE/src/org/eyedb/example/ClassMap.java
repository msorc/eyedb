package org.eyedb.example;

import java.util.AbstractMap;
import java.util.AbstractSet;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import org.eyedb.ClassIterator;
import org.eyedb.Oid;

class ClassMap extends AbstractMap< Oid, org.eyedb.Object> {

	ClassMap( EyeDBBean bean, org.eyedb.Class eyedbClass)
	{
		this.bean = bean;
		this.eyedbClass = eyedbClass;
	}
	
	@Override
	public Set<Map.Entry< Oid, org.eyedb.Object>> entrySet()
	{
		try {
			bean.openDatabase();
			bean.getDatabase().transactionBegin();

			Set<Map.Entry< Oid, org.eyedb.Object>> set;

			set = new HashSet<Map.Entry< Oid, org.eyedb.Object>>();

			ClassIterator i = new ClassIterator( eyedbClass);
			org.eyedb.Object o;

			while ((o = i.nextObject()) != null) {
				set.add( new AbstractMap.SimpleEntry< Oid, org.eyedb.Object>( o.getOid(), o) );
			}

			bean.getDatabase().transactionCommit();
			bean.closeDatabase();
			
			return set;
		}
		catch( org.eyedb.Exception e) {
			return null;
		}
	}

	public org.eyedb.Object get(Object key)
	{
		try {
			bean.openDatabase();
			bean.getDatabase().transactionBegin();

			Oid oid = new Oid((String)key);
			org.eyedb.Object obj = bean.getDatabase().loadObject( oid);

			bean.getDatabase().transactionCommit();
			bean.closeDatabase();

			return obj;
		}
		catch( org.eyedb.Exception e) {
		}

		return null;
	}

	private EyeDBBean bean;
	private org.eyedb.Class eyedbClass;
}
