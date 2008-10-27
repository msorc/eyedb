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
import org.polepos.circuits.bahrain.BahrainDriver;
import org.polepos.teams.jorm.data.JormPilot;



public class BahrainJorm extends JormDriver implements BahrainDriver{
    
    public void write(){
        

        int numobjects = setup().getObjectCount();
        int commitinterval = setup().getCommitInterval();
        
        int commitctr = 0;
        for ( int i = 1; i <= numobjects; i++ ){

            JormPilot p = new JormPilot( "Pilot_" + i, "Jonny_" + i, i , i );
            db().setObject( p );
            
            if ( commitinterval > 0  &&  ++commitctr >= commitinterval ){
                commitctr = 0;
                commit();
            }
            addToCheckSum(i);
        }
        
        commit();
    }
    
    
 
    public void query_indexed_string() {
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            JormPilot jp = (JormPilot) db().getObject( new Query(JormPilot.class,"mName",  Query.EQ , "Pilot_"+i));
            addToCheckSum(jp.checkSum());
        }
    }
    
    
            
    public void query_string() {
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            JormPilot jp = (JormPilot) db().getObject( new Query(JormPilot.class,"mFirstName",  Query.EQ , "Jonny_"+i));
            addToCheckSum(jp.checkSum());
        }
    }

    public void query_indexed_int() {
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            Query query = new Query(JormPilot.class,"mLicenseID",  Query.EQ , i);
            JormPilot jp = (JormPilot) db().getObject( query);
            addToCheckSum(jp.checkSum());
        }
    }

    public void query_int() {
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            Query query = new Query(JormPilot.class,"mPoints",  Query.EQ , i);
            JormPilot jp = (JormPilot) db().getObject( query);
            addToCheckSum(jp.checkSum());
        }
    }

    public void update() {
        int updateCount = setup().getUpdateCount();
        int commitinterval = setup().getCommitInterval();

        OidIterator oidIterator = db().getOidIterator(new Query(JormPilot.class));
        for (int i = 0; i < updateCount; i++) {
            JormPilot p = (JormPilot) oidIterator.next();
            p.setName( p.getName().toUpperCase() );
            addToCheckSum(1);
            if(i%commitinterval==0){
                commit();
            }
        }
        commit();
    }
    
    public void delete() {
        addToCheckSum(db().getObjectCount(JormPilot.class));
        db().deleteBulk(new Query(JormPilot.class));
        commit();
    }

}
