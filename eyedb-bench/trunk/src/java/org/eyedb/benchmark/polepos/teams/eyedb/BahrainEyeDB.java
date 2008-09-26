package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.Iterator;
import org.eyedb.benchmark.polepos.teams.eyedb.data.IndexedPilot;
import org.polepos.circuits.bahrain.BahrainDriver;

public class BahrainEyeDB extends EyeDBDriver implements BahrainDriver {

    public void write()
    {
        try {
	    getDatabase().transactionBegin();
            
            int count = setup().getObjectCount(); 
            int commitInterval = setup().getCommitInterval();
            int commitCount = 0;

            for ( int i = 0; i < count; i++ ){
		IndexedPilot p = new IndexedPilot( getDatabase());
		p.setName( "Pilot_" + (i+1));
		p.setFirstName( "Johnny_" + (i+1));
		p.setPoints(i+1);
		p.setLicenseID(i+1);
                
		p.store( org.eyedb.RecMode.FullRecurs);

                if ( commitInterval > 0  &&  ++commitCount >= commitInterval ) {
                    commitCount = 0;
		    getDatabase().transactionCommit();
                }
                
                addToCheckSum(i);
	    }

	    getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }

    public void query_indexed_string()
    {
        try {
	    int count = setup().getSelectCount();
        
	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select p from IndexedPilot as p where p.name=\"Pilot_" + (i+1) + "\"");

		while (it.hasNext()) {
		    IndexedPilot p = (IndexedPilot)it.next();
		    addToCheckSum( p.getPoints());
		}	       
	    }
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }

    public void query_string()
    {
        try {
	    int count = setup().getSelectCount();
        
	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select p from IndexedPilot as p where p.firstName=\"Johnny_" + (i+1) + "\"");

		while (it.hasNext()) {
		    IndexedPilot p = (IndexedPilot)it.next();
		    addToCheckSum( p.getPoints());
		}	       
	    }
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
    
    public void query_indexed_int()
    {
        try {
	    int count = setup().getSelectCount();
        
	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select p from IndexedPilot as p where p.licenseID=" + (i+1));

		while (it.hasNext()) {
		    IndexedPilot p = (IndexedPilot)it.next();
		    addToCheckSum( p.getPoints());
		}	       
	    }
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
    
    public void query_int()
    {
        try {
	    int count = setup().getSelectCount();
        
	    for (int i = 0; i < count; i++) {
		Iterator it = iterate( "select p from IndexedPilot as p where p.points=" + (i+1));

		while (it.hasNext()) {
		    IndexedPilot p = (IndexedPilot)it.next();
		    addToCheckSum( p.getPoints());
		}	       
	    }
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }

    public void update()
    {
        try {
            int updateCount = setup().getUpdateCount();

	    getDatabase().transactionBegin();

	    Iterator it = iterate( "select p from IndexedPilot as p");
            for (int i = 0; i < updateCount; i++) {
		IndexedPilot p = (IndexedPilot) it.next();

		p.setName( p.getName().toUpperCase() );
		p.store( org.eyedb.RecMode.FullRecurs);

                addToCheckSum(1);
	    }

	    getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }

    public void delete()
    {
	try {
	    getDatabase().transactionBegin();

	    Iterator it = iterate( "select p from IndexedPilot as p");
	    while (it.hasNext()) {
		IndexedPilot p = (IndexedPilot) it.next();
		p.remove();
		addToCheckSum( 1);
	    }

            getDatabase().transactionCommit();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	}
    }
}
