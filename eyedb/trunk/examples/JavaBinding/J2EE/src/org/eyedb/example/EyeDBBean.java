package org.eyedb.example;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.eyedb.Connection;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Oid;
import org.eyedb.RecMode;
import org.eyedb.Root;
import org.eyedb.example.schema.Database;

/**
 * @author Francois Dechelle <francois@dechelle.net>
 *
 */

public class EyeDBBean {
	private class ObjectMap extends AbstractMap< Oid, org.eyedb.Object> {

		public Set<Map.Entry< Oid, org.eyedb.Object>> entrySet()
		{
			throw new UnsupportedOperationException();
		}

		public org.eyedb.Object get(Object key)
		{
			try {
				openDatabase();
				getDatabase().transactionBegin();

				Oid oid = new Oid((String)key);
				org.eyedb.Object obj = getDatabase().loadObject( oid, RecMode.FullRecurs);

				getDatabase().transactionCommit();
				closeDatabase();

				return obj;
			}
			catch( org.eyedb.Exception e) {
			}

			return null;
		}
	}

	public EyeDBBean() throws org.eyedb.Exception
	{
		this.databaseName = "";
		this.tcpPort = "6240";

		String[] args = new String[3];
		int i = 0;
		args[i++] = "--user=" + System.getProperty( "user.name");
		args[i++] = "--dbm=default";

		Root.init( databaseName, args);

		Database.init();
	}

	public String getDatabaseName()
	{
		return databaseName;
	}

	public void setDatabaseName(String databaseName)
	{
		this.databaseName = databaseName;
	}

	public String getTcpPort()
	{
		return tcpPort;
	}

	public void setTcpPort(String tcpPort)
	{
		this.tcpPort = tcpPort;
	}

	public void openDatabase() throws org.eyedb.Exception
	{
		connection = new Connection( "localhost", tcpPort);
		database = new Database( databaseName);
		database.open(connection, Database.DBRW);
	}

	public void closeDatabase() throws org.eyedb.Exception
	{
		database.close();
		connection.close();
	}

	public Database getDatabase()
	{
		return database;
	}

	public Map getObjects() throws org.eyedb.Exception
	{
		return new ObjectMap();
	}

	private List<org.eyedb.Object> getClassObjects( String name) throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		List<org.eyedb.Object> result = new ArrayList<org.eyedb.Object>();

		OQL q = new org.eyedb.OQL( getDatabase(), "select o from " + name + " o");
		ObjectArray a = new ObjectArray();
		q.execute( a, RecMode.FullRecurs);

		for (int i = 0; i < a.getCount(); i++) {
			result.add( a.getObject(i));
		}

		getDatabase().transactionCommit();
		closeDatabase();

		return result;
	}
	
	public List<org.eyedb.Object> getPersons() throws org.eyedb.Exception
	{
		return getClassObjects( "Person");
	}

	public List<org.eyedb.Object> getCars() throws org.eyedb.Exception
	{
		return getClassObjects( "Car");
	}

	private String databaseName;
	private String tcpPort;

	private Database database;
	private Connection connection;
}
