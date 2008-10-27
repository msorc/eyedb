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



import java.sql.ResultSet;
import java.sql.SQLException;

import org.polepos.circuits.melbourne.*;
import org.polepos.data.*;
import org.polepos.framework.*;
import org.polepos.teams.jdbc.drivers.melbourne.*;

/**
 * @author Herkules
 */
public class MelbourneJdbc extends JdbcDriver implements MelbourneDriver
{
	/**
	 * Number of pilot to be written at once.
	 */
	private final static int BULKSIZE = 1000;

	
	public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{
		super.takeSeatIn(car, setup);
		
		jdbcCar().openConnection();
        
        //
        // Create database structure
        //
        jdbcCar().dropTable( "australia" );
        jdbcCar().createTable( "australia", new String[]{ "ID", "Name", "FirstName", "Points", "LicenseID" }, 
					new Class[]{Integer.TYPE, String.class, String.class, Integer.TYPE, Integer.TYPE} );

        jdbcCar().close();
	}
	
	public void write(){
        
        int numobjects = setup().getObjectCount(); 
        int commitintervall = setup().getCommitInterval();
        int commitctr = 0;
        
		Pilot[] pilots = new Pilot[ BULKSIZE ];
		int idx = 0;
        
		//BulkWriteStrategy writer = new BulkWriteSingle();
		BulkWriteStrategy writer = new BulkWritePreparedStatement( jdbcCar(), "australia" );
		//BulkWriteStrategy writer = new BulkWriteMultiValue();
		
        for ( int i = 1; i <= numobjects; i++ )
        {
            Pilot p = new Pilot( "Pilot_" + i, "Herkules", i, i );
			pilots[ idx++ ] = p;
			if ( idx == BULKSIZE )
			{
				writer.savePilots("australia", pilots, idx, i - idx + 1 );
				idx = 0;
			}
            
            if ( commitintervall > 0  &&  ++commitctr >= commitintervall )
            {
                commitctr = 0;
                jdbcCar().commit();
                Log.logger.fine ( "commit while writing at " + i+1 ); //NOI18N
            }
            
            addToCheckSum(i);
        }
		
		// Write the rest
		writer.savePilots("australia", pilots, idx, numobjects - idx );

        jdbcCar().commit();
    }
    
	public void read(){      
        int numobjects = setup().getObjectCount();
        JdbcCar car = jdbcCar();
        ResultSet rs = null;
		try{
			rs = car.executeQuery("select * from australia");
			for ( int i = 0; i < numobjects; i++ ){
				rs.next();
				Pilot p = new Pilot( rs.getString( 2 ), rs.getString( 3 ), rs.getInt( 4 ), rs.getInt( 5 ) );
                addToCheckSum(p.getPoints());
			}
		}
		catch ( SQLException sqlex ){
			sqlex.printStackTrace();
		} finally {
			car.closeQuery(rs);
		}
	}
    
    public void read_hot() {
        read();
    }
    
	public void delete(){
        jdbcCar().executeSQL( "delete from australia" );
        jdbcCar().commit();
	}

}
