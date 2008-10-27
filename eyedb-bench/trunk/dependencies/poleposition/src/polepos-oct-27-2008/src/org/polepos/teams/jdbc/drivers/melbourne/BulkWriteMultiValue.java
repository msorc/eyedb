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
 * Performs a bulkwrite by employing multiple VALUES part in the SQL statement.
 * @author Herkules
 *
 * Typical:
 * BUNCH=10:   50000 objects: write 1062ms (0.02124ms/object), read 172ms (0.00344ms/object), delete 0ms (0.0ms/object)
 * BUNCH=100:  50000 objects: write 500ms (0.01ms/object), read 156ms (0.00312ms/object), delete 0ms (0.0ms/object)
 * BUNCH=1000: 50000 objects: write 360ms (0.0072ms/object), read 218ms (0.00436ms/object), delete 0ms (0.0ms/object)
 */
public class BulkWriteMultiValue implements BulkWriteStrategy
{
	private final JdbcCar mCar;

	/** 
	 * Creates a new instance of BulkWriteMultiValue.
	 */
	public BulkWriteMultiValue( JdbcCar car )
	{
		mCar = car;
	}

	/**
	 * Dump an array of pilots to the DB by creating a loooong SQL statement.
	 */
	public void savePilots( String tablename, Pilot[] p, int count, int index)
	{
		if ( count == 0 ) return;
        StringBuffer stmt = new StringBuffer( "insert into ");
        stmt.append(tablename);
        stmt.append(" (id,Name,FrontName,Points,LicenseID) values " );
		for ( int i = 0; i < count-1; i++ )
		{
			values( stmt, p[i], index++ );
			stmt.append(",");
		}
		values( stmt, p[count-1], index++ );
		
        mCar.executeSQL( stmt.toString() );
	}

	
	/**
	 * Helper.
	 */ 
	private void values( StringBuffer to, Pilot p, int idx )
	{
		to.append( "(" ).append( Integer.toString( idx ) ).append( ", '" );
		to.append( p.getName() ).append( "', '" );
		to.append( p.getFirstName() ).append( "', " );
		to.append( Integer.toString( p.getPoints() ) ).append( ", " );
		to.append( Integer.toString( p.getLicenseID() ) ).append( ")" );
	}
	
}
