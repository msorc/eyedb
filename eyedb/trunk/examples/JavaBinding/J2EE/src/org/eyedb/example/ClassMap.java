package org.eyedb.example;

import java.util.AbstractMap;
import java.util.AbstractSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import org.eyedb.Oid;

class ClassMap extends AbstractMap< Oid, org.eyedb.Object> {

	private class ClassEntrySet extends AbstractSet<Map.Entry< Oid, org.eyedb.Object>> {

		private class ClassIterator implements Iterator<Map.Entry< Oid, org.eyedb.Object>> {

			public boolean hasNext()
			{
				// TODO Auto-generated method stub
				return false;
			}

			public Map.Entry< Oid, org.eyedb.Object> next()
			{
				// TODO Auto-generated method stub
				return null;
			}

			public void remove()
			{
				throw new UnsupportedOperationException();
			}
		}
		@Override
		public Iterator<Map.Entry< Oid, org.eyedb.Object>> iterator()
		{
			return new ClassIterator();
		}

		@Override
		public int size()
		{
			throw new UnsupportedOperationException();
		}

	}

	ClassMap( EyeDBBean bean, String className)
	{
		this.bean = bean;
		this.className = className;
	}
	
	@Override
	public Set<Map.Entry< Oid, org.eyedb.Object>> entrySet()
	{
		// TODO Auto-generated method stub
		return null;
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
	private String className;
}
