package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.Iterator;
import org.eyedb.benchmark.polepos.teams.eyedb.barcelona.B4;
import org.polepos.circuits.barcelona.BarcelonaDriver;
import org.polepos.framework.CarMotorFailureException;

public class BarcelonaEyeDB extends EyeDBDriver implements BarcelonaDriver {
    
    public void prepare() throws CarMotorFailureException 
    {
	try {
	    org.eyedb.benchmark.polepos.teams.eyedb.barcelona.Database.init();
	    
	    getEyeDBCar().openConnection( new org.eyedb.benchmark.polepos.teams.eyedb.barcelona.Database( "barcelona"));
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
            throw new CarMotorFailureException();
	}
    }

    public void write()
    {
        try {
	    getDatabase().transactionBegin();
            
            int count = setup().getObjectCount(); 

            for ( int i = 0; i < count; i++) {
                B4 b4 = new B4( getDatabase());

                b4.setB0( i+1);
                b4.setB1( i+1);
                b4.setB2( i+1);
                b4.setB3( i+1);
                b4.setB4( i+1);

		b4.store( org.eyedb.RecMode.FullRecurs);
            }
            
            getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
    
    public void read()
    {
	try {
	    Iterator it = iterate( "select b from B4 as b");

	    while (it.hasNext()) {
		B4 b = (B4)it.next();
		addToCheckSum( b.getB4());
	    }
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }
    
    public void query()
    {
	try {
	    int count = setup().getSelectCount();

	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select b from B4 as b where b.b2=" + (i+1));

		while (it.hasNext()) {
		    B4 b = (B4)it.next();
		    addToCheckSum( b.getB4());
		}	       
	    }
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }
    
    public void delete()
    {
	try {
	    getDatabase().transactionBegin();

	    Iterator it = iterate( "select b from B4 as b");

	    while (it.hasNext()) {
		B4 b = (B4)it.next();
		b.remove();
		addToCheckSum( 5);
	    }

            getDatabase().transactionCommit();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }
}
