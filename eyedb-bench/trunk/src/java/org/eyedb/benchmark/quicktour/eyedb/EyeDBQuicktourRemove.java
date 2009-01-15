package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Struct;
import org.eyedb.benchmark.quicktour.QuicktourRemove;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Course;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Student;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Teacher;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyeDBQuicktourRemove extends QuicktourRemove {

	public String getImplementation()
	{
		return "EyeDB Java implementation";
	}

	protected Database getDatabase()
	{
		return implementation.getDatabase();
	}

	public void prepare()
	{
		String databaseName = getProperties().getProperty( "eyedb.database");
		int tcpPort = getProperties().getIntProperty( "eyedb.tcp_port");

		implementation = new EyeDBImplementation( databaseName, tcpPort);
	}

	public void finish()
	{
		implementation.finish();
	}

	private void removeByClass( String className)
	{
		try {
			getDatabase().transactionBegin();

			String q = "select x from " + className + " as x";
			OQL query = new OQL( getDatabase(), q);

			ObjectArray result = new ObjectArray();
			query.execute( result);

			int count = result.getCount();
			Object[] objects = result.getObjects();

			for (int i = 0; i < count; i++) {
				Struct x = (Struct)objects[i];
				x.remove();
			}

			getDatabase().transactionCommit();
		}
		catch( org.eyedb.Exception e) {
			e.printStackTrace();
		}
	}

	public void remove()
	{
		removeByClass( "Teacher");
		removeByClass( "Course");
		removeByClass( "Student");
	}

	private EyeDBImplementation implementation;
}
