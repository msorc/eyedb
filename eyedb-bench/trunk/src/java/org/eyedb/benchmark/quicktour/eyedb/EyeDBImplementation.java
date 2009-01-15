package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Database;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

class EyeDBImplementation {

	EyeDBImplementation( String databaseName, int tcpPort)
	{
		String[] args = new String[3];
		int i = 0;
		args[i++] = "--user=" + System.getProperty( "user.name");
		args[i++] = "--dbm=default";
		args[i++] = "--port=" + tcpPort;

		org.eyedb.Root.init(databaseName, args);

		try {
			// Initialize the package
			org.eyedb.benchmark.quicktour.eyedb.quicktour.Database.init();

			// Open the connection with the backend
			connection = new org.eyedb.Connection();

			// Open the database
			database = new org.eyedb.benchmark.quicktour.eyedb.quicktour.Database( databaseName);
			database.open(connection, org.eyedb.Database.DBRW);
		}
		catch(org.eyedb.Exception e) { // Catch any eyedb exception
			e.printStackTrace();
			System.exit(1);
		}
	}

	Database getDatabase()
	{
		return database;
	}

	void finish()
	{
		/*		System.out.println( "org.eyedb.RPClib.read_ms = " + org.eyedb.RPClib.read_ms);
		System.out.println( "org.eyedb.RPClib.write_ms = " + org.eyedb.RPClib.write_ms);
		System.out.println( "org.eyedb.RPClib.write_async_ms = " + org.eyedb.RPClib.write_async_ms);
		System.out.println( "org.eyedb.RPClib.read_cnt = " + org.eyedb.RPClib.read_cnt);
		System.out.println( "org.eyedb.RPClib.read_async_cnt = " + org.eyedb.RPClib.read_async_cnt);
		 */
		try {
			// Close the database
			database.close();

			// Close the connection
			connection.close();
		}
		catch(org.eyedb.Exception e) { // Catch any eyedb exception
			e.printStackTrace();
			System.exit(1);
		}
	}

	protected org.eyedb.Connection connection;
	protected org.eyedb.benchmark.quicktour.eyedb.quicktour.Database database;
}
