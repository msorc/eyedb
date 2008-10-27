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

import org.polepos.circuits.imola.ImolaDriver;
import org.polepos.teams.jorm.data.JormPilot;


public class ImolaJorm extends JormDriver implements ImolaDriver{
    
    private Object[] oids;
    
    public void store() {
        int count = setup().getObjectCount();
        oids = new Object[setup().getSelectCount()];
        for ( int i = 1; i <= count; i++ ){
            storePilot(i);
        }
        commit();
    }

    public void retrieve() {
        for(Object oid : oids) {
            JormPilot pilot=(JormPilot)db().getObject((String) oid);
            if(pilot==null) {
                System.err.println("Object not found by ID.");
            }else{
                addToCheckSum(pilot.getPoints());
            }
        }   
    }

    private void storePilot(int idx) {
        JormPilot pilot = new JormPilot( "Pilot_" + idx, "Jonny_" + idx, idx , idx );
        db().setObject( pilot );
        
        if(idx <= setup().getSelectCount()) {
            oids[idx - 1] =  db().getOidString(pilot);
        }
        if ( isCommitPoint(idx) ){
            db().commit();
        }
    }

    private boolean isCommitPoint(int idx) {
        int commitInterval=setup().getCommitInterval();
        return commitInterval> 0  &&  idx%commitInterval==0 && idx<setup().getObjectCount();
    }

}
