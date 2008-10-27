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

package org.polepos.teams.jdo;


import java.util.Iterator;
import javax.jdo.Extent;

import org.polepos.circuits.melbourne.*;
import org.polepos.teams.jdo.data.*;

/**
 * @author Herkules
 */
public class MelbourneJdo extends JdoDriver implements MelbourneDriver{
	
	public void write(){
        
		begin();
        
        int numobjects = setup().getObjectCount();
        int commitinterval = setup().getCommitInterval();
		
		int commitctr = 0;
		for ( int i = 1; i <= numobjects; i++ ){
            
			JdoPilot p = new JdoPilot( "Pilot_" + i, i );
			db().makePersistent( p );
			
			if ( commitinterval > 0  &&  ++commitctr >= commitinterval ){
				commitctr = 0;
				commit();
				begin();
				Log.logger.fine( "commit while writing at " + i+1 ); //NOI18N
			}
            addToCheckSum(i);
		}
		
		commit();
	}
	
	public void read(){
        readExtent(JdoPilot.class);
	}
    
    public void read_hot() {
        read();
    }
    
	public void delete(){
        
        int numobjects =setup().getObjectCount();
        int commitinterval = setup().getCommitInterval();

		begin();
		
		Extent extent = db().getExtent( JdoPilot.class, false );
		Iterator itr = extent.iterator();
		int commitctr = 0;
		for ( int i = 0; i < numobjects; i++ ){
            
			JdoPilot p = (JdoPilot)itr.next();
			db().deletePersistent( p );
            
			if ( commitinterval > 0  && ++commitctr >= commitinterval ){
				commitctr = 0;
                
                // Debugging VOA: commit() seems to close the extent anyway
                // so we can do it here
                extent.closeAll();
				
                commit();
				begin();
				Log.logger.fine( "commit while deleting at " + i+1 ); //NOI18N
                
                // Debugging VOA: If we close the extent, we have to open it
                // again of course.
                extent = db().getExtent( JdoPilot.class, false );
                itr = extent.iterator();
			}
		}
        
		commit();
		
		extent.closeAll();
	}
}
