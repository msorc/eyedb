package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.ArrayList;
import java.util.List;

import org.eyedb.Oid;
import org.eyedb.benchmark.polepos.teams.eyedb.data.IndexedPilot;
import org.polepos.circuits.imola.ImolaDriver;
import org.polepos.framework.Car;
import org.polepos.framework.CarMotorFailureException;
import org.polepos.framework.TurnSetup;

public class ImolaEyeDB extends EyeDBDriver implements ImolaDriver {

    //    private Oid[] oids;
    private List<Oid> oids;

    public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException
    {
	//	oids = new Oid[setup.getSelectCount()];
	oids = new ArrayList<Oid>();
	super.takeSeatIn(car, setup);
    }

    private boolean isCommitPoint( int i)
    {
	int commitInterval = setup().getCommitInterval();

	return commitInterval > 0  &&  i % commitInterval == 0;
    }

    public void store()
    {
        try {
	    getDatabase().transactionBegin();

            for ( int i = 0; i < setup().getObjectCount(); i++ )
		storePilot(i);

	    getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }

    private void storePilot( int i) throws org.eyedb.Exception
    {
	IndexedPilot p = new IndexedPilot( getDatabase());
	p.setName( "Pilot_" + (i+1));
	p.setFirstName( "Johnny_" + (i+1));
	p.setPoints(i+1);
	p.setLicenseID(i+1);
                
	p.store( org.eyedb.RecMode.FullRecurs);
	
	if (isCommitPoint(i)) {
	    getDatabase().transactionCommit();
	    getDatabase().transactionBegin();
	}

	if ( i < setup().getSelectCount())
	    //	    oids[i] = p.getOid();
	    oids.add( p.getOid());
   }

    public void retrieve()
    {
        try {
	    getDatabase().transactionBegin();

	    for (Oid oid: oids) {
		IndexedPilot pilot = (IndexedPilot)getDatabase().loadObject( oid);

                addToCheckSum(pilot.getPoints());
	    }

	    getDatabase().transactionCommit();
        }
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
}
