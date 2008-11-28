package org.eyedb.benchmark.polepos.teams.db4o;

import org.polepos.circuits.barcelona.B4;

import com.db4o.ObjectSet;
import com.db4o.query.Query;

public class BarcelonaDb4o extends org.polepos.teams.db4o.BarcelonaDb4o {

    public void write()
    {
	String p = System.getProperties().getProperty("barcelona.write");

	if (p != null && p.equals("true"))
	    super.write();
    }

    public void read()
    {
	String p = System.getProperties().getProperty("barcelona.read");

	if (p != null && p.equals("true")) {
	    Query q = db().query();
	    q.constrain( B4.class);

	    ObjectSet result = doQuery( q);

// 	    ObjectSet result = q.execute();

// 	    while (result.hasNext()) {
// 		Object o = result.next();
// 		if (o instanceof CheckSummable) {
// 		    addToCheckSum(((CheckSummable) o).checkSum());
// 		}
// 	    }

	    System.err.println( "Read: " + result.size() + " objects of class " + B4.class.getName());
	}
    }

    public void query()
    {
	String p = System.getProperties().getProperty("barcelona.query");

	if (p != null && p.equals("true"))
	    super.query();
    }

    public void delete()
    {
	String p = System.getProperties().getProperty("barcelona.delete");

	if (p != null && p.equals("true"))
	    super.delete();
    }
}
