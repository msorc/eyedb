package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.Iterator;
import org.eyedb.Database;
import org.eyedb.ObjectArray;
import org.eyedb.OQL;
import org.polepos.framework.Driver;
import org.polepos.framework.CarMotorFailureException;

public abstract class EyeDBDriver extends Driver {

    public void prepare() throws CarMotorFailureException 
    {
	try {
	    org.eyedb.benchmark.polepos.teams.eyedb.data.Database.init();
	    
	    getEyeDBCar().openConnection( new org.eyedb.benchmark.polepos.teams.eyedb.data.Database( "poleposition"));
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
            throw new CarMotorFailureException();
	}
    }

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

    class QueryIterator implements Iterator {
	QueryIterator( OQL query) throws org.eyedb.Exception
	{
	    ObjectArray result = new ObjectArray();

	    query.execute( result);

	    i = 0;
	    count = result.getCount();
	    objects = result.getObjects();
	}

	public boolean hasNext()
	{
	    return i < count;
	}

	public Object next()
	{
	    return objects[i++];
	}

	public void remove()
	{
	    throw new UnsupportedOperationException();
	}

	private int i;
	private int count;
	private Object[] objects;
    }

    Iterator iterate( String q) throws org.eyedb.Exception
    {
	return new QueryIterator( new OQL( getDatabase(), q));
    }
}
