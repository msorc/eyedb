package org.eyedb.benchmark.polepos.teams.eyedb;

import org.eyedb.benchmark.polepos.teams.eyedb.barcelona.Database;
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
    }
    
    public void query()
    {
    }
    
    public void delete()
    {
    }
}
