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

import java.sql.*;
import java.util.*;

import org.polepos.data.*;
import org.polepos.framework.*;


/**
 * @author Herkules
 */
public abstract class JdbcDriver extends org.polepos.framework.Driver {
    
	public void prepare() throws CarMotorFailureException{
		((JdbcCar)car()).openConnection();
	}
	
	public void backToPit(){
        ((JdbcCar)car()).close();
	}
    
    public JdbcCar jdbcCar(){
        return (JdbcCar)car();
    }

	/**
	 * Helper: perform any query
	 */
	protected void performQuery(String sql) {
		Log.logger.fine("starting query"); // NOI18N
		JdbcCar car = jdbcCar();
		ResultSet rs = null;
		try {
			rs = car.executeQuery(sql);
			while (rs.next()) {
				Pilot p = new Pilot(rs.getString(2), rs.getString(3), rs
						.getInt(4), rs.getInt(5));
				addToCheckSum(p.checkSum());
			}
		} catch (SQLException sqlex) {
			sqlex.printStackTrace();
		} finally {
			car.closeQuery(rs);
		}
	}

	protected <Value> void performSingleResultQuery(String sql,List<Value> values) {
	    Log.logger.fine( "starting query" ); //NOI18N
	    PreparedStatement stat=jdbcCar().prepareStatement(sql);
		try {
			for(Value val : values) {
				stat.setObject(1,val);
				ResultSet rs=stat.executeQuery();
				if(!rs.next()) {
					System.err.println("Expected one result, received none: "+val);
				}
			    Pilot p = new Pilot( rs.getString( 2 ), rs.getString( 3 ), rs.getInt( 4 ), rs.getInt( 5 ) );
                addToCheckSum(p.checkSum());
				if(rs.next()) {
					System.err.println("Expected one result, received multiple: "+val);
				}
			}
		} catch (SQLException sqlexc) {
	        sqlexc.printStackTrace();
		}
		finally {
			try {
				stat.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}
    
    

}
