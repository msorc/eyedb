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

package org.polepos.teams.jdbc.drivers.melbourne;

import org.polepos.data.*;
import org.polepos.teams.jdbc.*;


/**
 *
 * @author Herkules
 *
 * Typical:
 * 50000 objects: write 4703ms (0.09406ms/object), read 172ms (0.00344ms/object), delete 0ms (0.0ms/object)
 */
public class BulkWriteSingle implements BulkWriteStrategy
{
	private final JdbcCar mCar;
	
	/** 
	 * Creates a new instance of BulkWriteSingle.
	 */
	public BulkWriteSingle( JdbcCar car )
	{
		mCar = car;
	}

	/**
	 * Dump an array of pilots to the DB by writing one-by-one.
	 */
	public void savePilots(String tablename, Pilot[] p, int count, int index )
	{
		for ( int i = 0; i < count; i++ )
		{
			savePilot( tablename, p[i], index++ );
		}		
	}
	
	
    /**
	 * Helper:
     * Write a single pilot to the database.
     */
    private void savePilot( String tablename, Pilot p, int index )
    {
        StringBuffer stmt = new StringBuffer( "insert into ");
        stmt.append(tablename);
        stmt.append(" (id,Name,FrontName,Points,LicenseID) values (");
        stmt.append( Integer.toString( index ) ).append( ", '" );
		stmt.append( p.getName() ).append( "', '" );
		stmt.append( p.getFirstName() ).append( "', " );
		stmt.append( Integer.toString( p.getPoints() ) ).append( ", " );
		stmt.append( Integer.toString( p.getLicenseID() ) ).append( ")" );

		mCar.executeSQL( stmt.toString() );
    }
}
