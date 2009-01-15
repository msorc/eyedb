package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Struct;
import org.eyedb.benchmark.quicktour.QuicktourQuery;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyeDBQuicktourQuery extends QuicktourQuery {

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

	private void queryByClass( String className, int nSelects)
	{
		try {
			getDatabase().transactionBegin();

			for ( int n = 0;  n < nSelects; n++) {
				String q = "select x from " + className + " as x where x.lastName ~ \"" + n + "\"";
				OQL query = new OQL( getDatabase(), q);

				ObjectArray result = new ObjectArray();
				query.execute( result);

				int count = result.getCount();
				Object[] objects = result.getObjects();

				for (int i = 0; i < count; i++) {
					Struct x = (Struct)objects[i];
				}
			}

			getDatabase().transactionCommit();
		}
		catch( org.eyedb.Exception e) {
			e.printStackTrace();
		}
	}

	public void query( int nSelects)
	{
		queryByClass( "Teacher", nSelects);
		queryByClass( "Student", nSelects);
	}

	private EyeDBImplementation implementation;
}
