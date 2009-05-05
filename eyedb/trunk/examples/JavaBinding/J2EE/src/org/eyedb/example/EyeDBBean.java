package org.eyedb.example;

import org.eyedb.Connection;
import org.eyedb.Root;
import org.eyedb.example.schema.Database;

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

	private void openDatabase() throws org.eyedb.Exception
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
	
	public Database getDatabase() throws org.eyedb.Exception
	{
		if (database == null)
			openDatabase();
		return database;
	}

	protected void closeDatabase() throws org.eyedb.Exception
	{
		database.close();
		connection.close();
	}

	private String databaseName;
	private String tcpPort;
	private Database database;
	private Connection connection;
}
