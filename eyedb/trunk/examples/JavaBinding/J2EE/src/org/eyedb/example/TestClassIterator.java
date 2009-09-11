package org.eyedb.example;

import org.eyedb.ClassIterator;
import org.eyedb.example.schema.Person;

public class TestClassIterator {

	TestClassIterator( EyeDBBean bean, org.eyedb.Class eyedbClass) throws org.eyedb.Exception
	{
		bean.openDatabase();
		bean.getDatabase().transactionBegin();

		ClassIterator i = new ClassIterator( eyedbClass);
		org.eyedb.Object o;

		while ((o = i.nextObject()) != null) {
			o.trace();
		}

		bean.getDatabase().transactionCommit();
		bean.closeDatabase();
	}
	
	public static void main(String[] args)
	{
		try {
			EyeDBBean bean = new EyeDBBean();

			bean.setDatabaseName( "testj2ee");
			
			new TestClassIterator( bean, Person.idbclass);
		}
		catch( org.eyedb.Exception e)  {
			e.printStackTrace();
		}
	}

}
