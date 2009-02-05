package org.eyedb.jodl;

import java.util.Iterator;

import org.eyedb.Database;

public class JODL {

    public JODL( Database database)
    {
	this.database = database;
    }

    public void print()
    {
	Iterator it = database.getSchema().getClassList().iterator();

	while( it.hasNext()) {
	    org.eyedb.Class cl = (org.eyedb.Class)it.next();
	    if (!cl.isSystem())
		System.out.println( cl.getName());
	}
    }

    public String toString()
    {
	return getClass().getName() + ": database=" + database.getName();
    }

    private Database database;
}
