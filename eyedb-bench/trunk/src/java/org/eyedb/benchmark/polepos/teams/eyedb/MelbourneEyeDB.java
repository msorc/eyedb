package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.Iterator;
import org.eyedb.benchmark.polepos.teams.eyedb.data.Pilot;
import org.polepos.circuits.melbourne.MelbourneDriver;

public class MelbourneEyeDB extends EyeDBDriver implements MelbourneDriver {
    
    public void write( )
    {
	try {
	    getDatabase().transactionBegin();
			
            int count = setup().getObjectCount(); 
            int commitInterval = setup().getCommitInterval();
            int commitCount = 0;

	    for ( int i = 0; i < count; i++ ) {
		Pilot p = new Pilot( getDatabase());
		p.setName( "Pilot_" + (i+1));
		p.setFirstName( "Herkules");
		p.setPoints( i+1);
		p.setLicenseID( 1);
		
		p.store( org.eyedb.RecMode.FullRecurs);
                
		if ( commitInterval > 0  &&  ++commitCount >= commitInterval ) {
		    commitCount = 0;
		    getDatabase().transactionCommit();
		    getDatabase().transactionBegin();
		}

                addToCheckSum(i);
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
	    getDatabase().transactionBegin();

	    int count = setup().getSelectCount();
        
	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select p from Pilot as p");

		while (it.hasNext()) {
		    Pilot p = (Pilot)it.next();
		    addToCheckSum( p.getPoints());
		}	       
	    }

	    getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
	
    public void read_hot()
    {
        read();
    }

    public void delete()
    {
	try {
	    getDatabase().transactionBegin();

	    Iterator it = iterate( "select p from Pilot as p");
	    while (it.hasNext()) {
		Pilot p = (Pilot) it.next();
		p.remove();
	    }

            getDatabase().transactionCommit();
	} 
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
}
