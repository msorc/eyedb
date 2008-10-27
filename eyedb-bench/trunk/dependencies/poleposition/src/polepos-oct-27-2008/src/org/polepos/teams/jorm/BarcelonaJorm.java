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
import org.polepos.circuits.barcelona.BarcelonaDriver;
import org.polepos.framework.CheckSummable;
import org.polepos.teams.jorm.data.JB4;



public class BarcelonaJorm extends JormDriver implements BarcelonaDriver{
    
    public void write(){
        int count = setup().getObjectCount();
        for (int i = 1; i<= count; i++) {
            JB4 b4 = new JB4();
            b4.setAll(i);
            store(b4);
        }
        commit();
    }
    
    public void read(){
        OidIterator list = db().getObjects(JB4.class);
        for (int i = 0; i < list.size(); i++) {
            JB4 jb4 = (JB4) list.get(i);
            addToCheckSum(jb4.checkSum());
        }
    }
    
    public void query(){
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            OidIterator list = db().getObjects(new Query(JB4.class, "b2", Query.EQ, i ));
            for (int j = 0; j < list.size(); j++) {
                Object o = (Object) list.get(j);
                if(o instanceof CheckSummable){
                    addToCheckSum(((CheckSummable)o).checkSum());
                }
            }
        }
    }
    
    public void delete(){
        int count = db().getObjectCount(JB4.class);
        db().deleteBulk(new Query(JB4.class));
        addToCheckSum(count*5);
        commit();
    }

}
