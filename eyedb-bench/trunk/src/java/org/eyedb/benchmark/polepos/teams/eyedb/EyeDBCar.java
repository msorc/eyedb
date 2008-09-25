package org.eyedb.benchmark.polepos.teams.eyedb;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.Root;
import org.polepos.framework.Car;

public class EyeDBCar extends Car {

    private Connection connection;
    private Database database;

    void openConnection( Database database) throws org.eyedb.Exception
    {
	Root.init( "", new String[] { "--user=" + System.getProperty( "user.name")});

	connection = new Connection();
	database.open(connection, Database.DBRW);

	this.database = database;
    }

    void closeConnection() throws org.eyedb.Exception
    {
	database.close();
	connection.close();
    }

    Database getDatabase()
    {
	return database;
    }

    public String name()
    {
	return "EyeDB";
    }
}
