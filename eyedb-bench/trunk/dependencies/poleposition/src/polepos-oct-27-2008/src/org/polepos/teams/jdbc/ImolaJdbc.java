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

package org.polepos.teams.jdbc;

import java.util.*;

import org.polepos.circuits.imola.*;
import org.polepos.data.*;
import org.polepos.framework.*;
import org.polepos.teams.jdbc.drivers.melbourne.*;


public class ImolaJdbc extends JdbcDriver implements ImolaDriver {
	private final static int BULKSIZE = 1000;
    private static final String TABLE = "sanmarino";

	public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException
	{	
		super.takeSeatIn(car, setup);

        jdbcCar().openConnection();
        jdbcCar().dropTable( TABLE);
        jdbcCar().createTable( TABLE, new String[]{ "id", "Name", "FirstName", "Points", "LicenseID" }, 
					new Class[]{Integer.TYPE, String.class, String.class, Integer.TYPE, Integer.TYPE} );
        jdbcCar().close();
	}

	public void store() {
		Pilot[] pilots = new Pilot[ BULKSIZE ];		
		BulkWriteStrategy writer = new BulkWritePreparedStatement(jdbcCar(), TABLE);
        for ( int i = 0; i < setup().getObjectCount(); i++ ){
			storePilot(pilots, writer, i+1);
		}
        jdbcCar().commit();
	}
	
	public void retrieve() {
		List<Integer> ids=new ArrayList<Integer>(setup().getSelectCount());
		for(int id=1;id<=setup().getSelectCount();id++) {
			ids.add(id);
		}
		performSingleResultQuery( "select * from "+TABLE+" where id=?",ids );
	}

	private void storePilot(Pilot[] pilots, BulkWriteStrategy writer, int idx) {
		int bulkidx=(idx-1)%BULKSIZE;
		pilots[ bulkidx ] = new Pilot( "Pilot_" + idx, "Jonny_" + idx, idx , idx );
		if ( isBulkWritePoint(idx, bulkidx)){
			writer.savePilots(TABLE, pilots, bulkidx+1, idx - bulkidx );
		    Log.logger.fine( "bulk write after writing at " + idx ); //NOI18N
		}
		if ( isCommitPoint(idx)){
		    jdbcCar().commit();
		    Log.logger.fine( "commit after writing at " + idx ); //NOI18N
		}
	}

	private boolean isBulkWritePoint(int idx, int bulkidx) {
		return (idx>1 && bulkidx == BULKSIZE-1) || idx == setup().getObjectCount();
	}

	private boolean isCommitPoint(int idx) {
		return setup().getCommitInterval() > 0  &&  idx%setup().getCommitInterval()==0 && idx<setup().getObjectCount();
	}
}
