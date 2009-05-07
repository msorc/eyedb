package org.eyedb.example;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.eyedb.Connection;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Oid;
import org.eyedb.RecMode;
import org.eyedb.Root;
import org.eyedb.example.schema.Database;
import org.eyedb.example.schema.Person;

/**
 * @author Francois Dechelle <francois@dechelle.net>
 *
 */

public class EyeDBBean {

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
		String[] args = new String[3];
		int i = 0;
		args[i++] = "--user=" + System.getProperty( "user.name");
		args[i++] = "--dbm=default";
		args[i++] = "--port=" + tcpPort;

		Root.init( databaseName, args);

		Database.init();

		connection = new Connection();
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

	public Map getPersons() throws org.eyedb.Exception
	{
		return new ClassMap( this, "Person");
	}
	
	private List<Person> getOldPersons() throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		List<Person> result = new ArrayList<Person>();
	
		OQL q = new org.eyedb.OQL( getDatabase(), "select p from Person p");
		ObjectArray a = new ObjectArray();
		q.execute( a, RecMode.FullRecurs);

		for (int i = 0; i < a.getCount(); i++) {
			Person p = (Person)a.getObject(i);
			result.add( p);
		}

		getDatabase().transactionCommit();
		closeDatabase();
		
		return result;
	}

	public Person createPerson() throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		Person person = new Person( getDatabase());

		getDatabase().transactionCommit();
		closeDatabase();
		
		return person;
	}

	public Person loadPerson( String oid) throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		Oid personOid = new Oid(oid);
		Person person = (Person)getDatabase().loadObject( personOid);

		getDatabase().transactionCommit();
		closeDatabase();
		
		return person;
	}

	public void storePerson( Person person) throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		person.store( RecMode.FullRecurs);

		getDatabase().transactionCommit();
		closeDatabase();
	}
	
	public void deletePerson( Person person) throws org.eyedb.Exception
	{
		openDatabase();
		getDatabase().transactionBegin();

		person.remove();

		getDatabase().transactionCommit();
		closeDatabase();
	}
	
	private String databaseName;
	private String tcpPort;
	
	private Database database;
	private Connection connection;
}
