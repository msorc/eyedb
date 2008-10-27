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

package org.polepos.teams.jorm;


import de.ama.db.OidIterator;
import de.ama.db.Query;
import org.polepos.circuits.melbourne.MelbourneDriver;
import org.polepos.teams.jorm.data.JormPilot;

/**
 * @author Andreas Marochow
 */
public class MelbourneJorm extends JormDriver implements MelbourneDriver{
	
	public void write(){
        
        int numobjects = setup().getObjectCount();
        int commitinterval  = setup().getCommitInterval();
		int commitctr = 0;

        for ( int i = 1; i <= numobjects; i++ ){

            JormPilot p = new JormPilot( "Pilot_" + i, "Herkules", i, i ) ;
            store(p);

            if ( commitinterval > 0  &&  ++commitctr >= commitinterval ){
                commitctr = 0;
                commit();
            }

            addToCheckSum(i);
        }

        commit();
	}
	
	public void read(){
        OidIterator list = db().getObjects(JormPilot.class);
        for (int i = 0; i < list.size(); i++) {
            JormPilot jormPilot = (JormPilot) list.get(i);
            addToCheckSum(jormPilot.checkSum());
        }

    }
    
    public void read_hot() {
        read();
    }
    
	public void delete(){
        db().deleteBulk(new Query(JormPilot.class));
		commit();
	}
}
