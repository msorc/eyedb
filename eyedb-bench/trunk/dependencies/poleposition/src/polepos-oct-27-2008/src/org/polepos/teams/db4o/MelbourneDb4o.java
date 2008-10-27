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

package org.polepos.teams.db4o;

import org.polepos.circuits.melbourne.*;
import org.polepos.data.*;

import com.db4o.*;
import com.db4o.query.*;


/**
 * @author Herkules
 */
public class MelbourneDb4o extends Db4oDriver implements MelbourneDriver{
    
	
	public void write(){
        
        int numobjects = setup().getObjectCount();
        int commitinterval  = setup().getCommitInterval();
		int commitctr = 0;
        
        begin();
		for ( int i = 1; i <= numobjects; i++ ){
            
            Pilot p =new Pilot( "Pilot_" + i, "Herkules", i, i ) ;
            store(p);
            
			if ( commitinterval > 0  &&  ++commitctr >= commitinterval ){
				commitctr = 0;
				commit();
                begin();
			}
            
            addToCheckSum(i);
		}
        
		commit();
	}

	public void read(){
        doQuery(Pilot.class);
	}

    public void read_hot() {
        read();
    }
	
	public void delete(){
        
        begin();
        
        int numobjects = setup().getObjectCount();
        int commitintervall = setup().getCommitInterval(); 
        
		int commitctr = 0;
		
        // By setting an activation depth of zero, we can 
        // prevent instantiating the Pilot objects.
        db().configure().activationDepth(0);
        
        Query allpilots = db().query();
		allpilots.constrain( Pilot.class );
		ObjectSet deleteset = allpilots.execute();
		
		for ( int i = 0; i < numobjects; i++ ){
			
            Object o = deleteset.next();
			
			db().delete(o);
			
			if ( commitintervall > 0  && ++commitctr >= commitintervall ){
				commitctr = 0;
				commit();
                begin();
			}
		}
		
		commit();
	}
	
}
