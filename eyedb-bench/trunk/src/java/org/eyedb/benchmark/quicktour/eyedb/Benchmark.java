package org.eyedb.benchmark.quicktour.eyedb;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class Benchmark extends org.eyedb.benchmark.framework.Benchmark {

    public void prepare()
    {
	String databaseName = getProperties().getProperty( "database");

	String[] args = new String[3];
	int i = 0;
	args[i++] = "--user=" + System.getProperty( "user.name");
	args[i++] = "--dbm=default";
	args[i++] = "--port=" + getProperties().getProperty( "tcp_port");

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

    public void finish()
    {
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
