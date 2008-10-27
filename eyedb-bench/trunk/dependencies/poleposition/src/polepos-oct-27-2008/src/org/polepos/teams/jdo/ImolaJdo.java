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

import org.polepos.circuits.imola.*;
import org.polepos.teams.jdo.data.*;


public class ImolaJdo extends JdoDriver implements ImolaDriver{
    
    private Object[] oids;
    
    public void store() {
        int count = setup().getObjectCount();
        oids = new Object[setup().getSelectCount()];
        begin();
        for ( int i = 1; i <= count; i++ ){
            storePilot(i);
        }
        commit();
    }

    public void retrieve() {
        for(Object id: oids) {
            JdoPilot pilot=(JdoPilot)db().getObjectById(id, false);
            if(pilot==null) {
                System.err.println("Object not found by ID.");
            }else{
                addToCheckSum(pilot.getPoints());
            }
        }   
    }

    private void storePilot(int idx) {
        JdoPilot pilot = new JdoPilot( "Pilot_" + idx, "Jonny_" + idx, idx , idx );
        db().makePersistent( pilot );
        
        if(idx <= setup().getSelectCount()) {
            oids[idx - 1] =  db().getObjectId(pilot);
        }
        if ( isCommitPoint(idx) ){
            db().currentTransaction().commit();
            db().currentTransaction().begin();
        }
    }

    private boolean isCommitPoint(int idx) {
        int commitInterval=setup().getCommitInterval();
        return commitInterval> 0  &&  idx%commitInterval==0 && idx<setup().getObjectCount();
    }


}
