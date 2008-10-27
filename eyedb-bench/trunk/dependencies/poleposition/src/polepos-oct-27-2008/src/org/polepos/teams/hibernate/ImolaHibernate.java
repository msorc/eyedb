/* 
This file is part of the PolePosition database benchmark
http://www.polepos.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA  02111-1307, USA. */

package org.polepos.teams.hibernate;

import java.io.*;

import org.polepos.circuits.imola.*;
import org.polepos.framework.*;
import org.polepos.teams.hibernate.data.*;

import net.sf.hibernate.*;

public class ImolaHibernate extends HibernateDriver implements ImolaDriver {
    private final String FROM = "from org.polepos.teams.hibernate.data.HibernateIndexedPilot";
	private Serializable[] ids;
	private int step;
	
	public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{	
		ids=new Serializable[setup.getSelectCount()];
		super.takeSeatIn(car, setup);
	}

	public void store() {
        try{
            Transaction tx = db().beginTransaction();
            for ( int i = 1; i <= setup().getObjectCount(); i++ ){
                storePilot(tx, i);
            }
            tx.commit();
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
	}

	public void retrieve() {
		for(Serializable id : ids) {
			try {
				HibernateIndexedPilot pilot=(HibernateIndexedPilot) db().load(HibernateIndexedPilot.class,id);
                addToCheckSum(pilot.getPoints());
				if(pilot==null) {
					System.err.println("Got no pilot by ID.");
				}
			} catch (HibernateException e) {
				e.printStackTrace();
			}
		}
	}

	private void storePilot(Transaction trans, int idx) throws HibernateException {
		Serializable id=db().save( new HibernateIndexedPilot( "Pilot_" + idx, "Jonny_" + idx, idx , idx ) );
		if ( isCommitPoint(idx) ){
		    trans.commit();
		    Log.logger.fine( "commit while writing at " + idx+1 ); //NOI18N
		}
        if(idx <= setup().getSelectCount()) {
            ids[idx - 1] =  id;
        }
	}

	private boolean isCommitPoint(int idx) {
		int commitInterval = setup().getCommitInterval();
		return commitInterval> 0  &&  idx%commitInterval==0 && idx < setup().getObjectCount();
	}
}
