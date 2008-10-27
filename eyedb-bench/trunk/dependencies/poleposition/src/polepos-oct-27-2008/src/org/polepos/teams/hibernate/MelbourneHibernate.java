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

import org.polepos.circuits.melbourne.*;
import org.polepos.teams.hibernate.data.*;

import net.sf.hibernate.*;

/**
 * @author Herkules
 */
public class MelbourneHibernate extends HibernateDriver implements MelbourneDriver{
    
    private final String FROM = "from org.polepos.teams.hibernate.data.HibernatePilot";    
	
	public void write( ){
        
        int numobjects = setup().getObjectCount();
        int commitintervall = setup().getCommitInterval();
        
		int commitctr = 0;
		try{
            
			Transaction tx = db().beginTransaction();
			
			for ( int i = 1; i <= numobjects; i++ ){
                
				HibernatePilot p = new HibernatePilot( "Pilot_" + i, "Herkules", i, 1 );
				
				db().save( p );
                
                addToCheckSum(i);
				
				if ( commitintervall > 0  &&  ++commitctr >= commitintervall )
				{
					commitctr = 0;
					tx.commit();
					tx = db().beginTransaction();
					Log.logger.fine( "commit while writing at " + i+1 ); //NOI18N
				}
			}
			tx.commit();
		}
		catch ( HibernateException hex ){
			hex.printStackTrace();
		}
	}
	
	public void read(){
        doQuery(FROM);
	}
	
    public void read_hot() {
        read();
    }

	public void delete(){
		try{
			Transaction tx = db().beginTransaction();
			db().delete(FROM);
			tx.commit();
		}catch ( HibernateException hex ){
			hex.printStackTrace();
		}
	}

}
