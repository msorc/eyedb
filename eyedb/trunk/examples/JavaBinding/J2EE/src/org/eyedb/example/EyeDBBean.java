package org.eyedb.example;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Logger;

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

	public static Logger logger = Logger.getLogger("org.eyedb");

	public void logObject( org.eyedb.Object obj)
	{
		EyeDBBean.logger.info( obj.toString());
		if (obj instanceof org.eyedb.example.schema.Person) {
			try {
				org.eyedb.example.schema.Person p = (org.eyedb.example.schema.Person)obj;
				EyeDBBean.logger.info(p.getFirstname() + " "
						+ p.getLastname() + " : getCarsCount() -> " + p.getCarsCount());
			} catch (org.eyedb.Exception e) {
			}				
		}
	}

	private class ObjectMap extends AbstractMap< Oid, Object> {

		ObjectMap()
		{
			this.mode = RecMode.NoRecurs;
		}
		
		ObjectMap( RecMode mode)
		{
			this.mode = mode;
		}
		
		public Set<Map.Entry< Oid, Object>> entrySet()
		{
			throw new UnsupportedOperationException();
		}

		public Object get(Object key)
		{
			try {
				openDatabase();
				getDatabase().transactionBegin();

				Oid oid = new Oid((String)key);
				org.eyedb.Object obj = getDatabase().loadObject( oid, mode);

				Object ret = wrapObject( obj);

				getDatabase().transactionCommit();
				closeDatabase();

				return ret;
			}
			catch( org.eyedb.Exception e) {
			}

			return null;
		}

		private RecMode mode;
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

	public Map getPerson() throws org.eyedb.Exception
	{
		return new ObjectMap();
	}

	public Map getCar() throws org.eyedb.Exception
	{
		return new ObjectMap(  RecMode.FullRecurs);
	}

	private Object wrapObject( org.eyedb.Object obj) throws org.eyedb.Exception
	{
		if (obj instanceof org.eyedb.example.schema.Person)
			return new org.eyedb.example.schema.fix.Person( (org.eyedb.example.schema.Person)obj);

		return obj;
	}

	private List<Object> getClassObjects( String name) throws org.eyedb.Exception
	{
		return getClassObjects( name, RecMode.NoRecurs);
	}
	
	private List<Object> getClassObjects( String name, RecMode mode) throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		List<Object> result = new ArrayList<Object>();

		OQL q = new org.eyedb.OQL( getDatabase(), "select o from " + name + " o");
		ObjectArray a = new ObjectArray();
		q.execute( a, mode);

		for (int i = 0; i < a.getCount(); i++) {
			result.add( wrapObject(a.getObject(i)));
		}

		getDatabase().transactionCommit();
		closeDatabase();

		return result;
	}

	public List<Object> getPersons() throws org.eyedb.Exception
	{
		return getClassObjects( "Person");
	}

	public List<Object> getCars() throws org.eyedb.Exception
	{
		return getClassObjects( "Car", RecMode.FullRecurs);
	}

	private String databaseName;
	private String tcpPort;

	private Database database;
	private Connection connection;
}
