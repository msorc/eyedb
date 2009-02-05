package org.eyedb.jodl;

import org.eyedb.Connection;
import org.eyedb.Database;

public class JODLMain {

    public static void main( String[] args)
    {
	String databaseName = args[0];

	int tcpPort = 6240;

	String[] initArgs = new String[3];
	int i = 0;
	initArgs[i++] = "--user=" + System.getProperty( "user.name");
	initArgs[i++] = "--dbm=default";
	initArgs[i++] = "--port=" + tcpPort;

	org.eyedb.Root.init( databaseName, initArgs);

	try {
	    Connection connection = new Connection();
	    Database database = new Database( databaseName);
	    database.open( connection, Database.DBRead);

	    JODL jodl = new JODL( database);
	    System.out.println( jodl);
	    jodl.print();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}

    }
}
