package org.eyedb.benchmark.polepos.teams.eyedb;

import org.eyedb.Database;
import org.polepos.framework.Driver;

public abstract class EyeDBDriver extends Driver {

    public void backToPit()
    {
	try {
	    getEyeDBCar().closeConnection();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }

    EyeDBCar getEyeDBCar()
    {
	return (EyeDBCar)car();
    }

    Database getDatabase()
    {
	return getEyeDBCar().getDatabase();
    }
}
