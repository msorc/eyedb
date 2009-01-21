package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Struct;
import org.eyedb.benchmark.quicktour.Quicktour;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class EyeDBQuicktour extends Quicktour {

    protected Database getDatabase()
    {
	return database;
    }

    public void finish()
    {
	try {
	    database.close();
	    connection.close();
	}
	catch(org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }

    private void queryByClass( String className, int nSelects)
    {
	try {
	    getDatabase().transactionBegin();

	    for ( int n = 0;  n < nSelects; n++) {
		String q = "select x from " + className + " as x where x.lastName = \"" + className + "_" + n + "\"";
		OQL query = new OQL( getDatabase(), q);

		ObjectArray result = new ObjectArray();
		query.execute( result);

		int count = result.getCount();
		Object[] objects = result.getObjects();

		for (int i = 0; i < count; i++) {
		    Struct x = (Struct)objects[i];
		}
	    }

	    getDatabase().transactionCommit();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }

    public void query( int nSelects)
    {
	queryByClass( "Teacher", nSelects);
	queryByClass( "Student", nSelects);
    }

    private void removeByClass( String className)
    {
	try {
	    getDatabase().transactionBegin();

	    String q = "select x from " + className + " as x";
	    OQL query = new OQL( getDatabase(), q);

	    ObjectArray result = new ObjectArray();
	    query.execute( result);

	    int count = result.getCount();
	    Object[] objects = result.getObjects();

	    for (int i = 0; i < count; i++) {
		Struct x = (Struct)objects[i];
		x.remove();
	    }

	    getDatabase().transactionCommit();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }

    public void remove()
    {
	removeByClass( "Teacher");
	removeByClass( "Course");
	removeByClass( "Student");
    }

    protected Database database;
    protected Connection connection;
}
