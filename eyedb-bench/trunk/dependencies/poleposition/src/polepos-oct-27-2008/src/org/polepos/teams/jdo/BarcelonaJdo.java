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

import java.util.*;

import javax.jdo.*;

import org.polepos.circuits.barcelona.*;
import org.polepos.teams.jdo.data.*;



public class BarcelonaJdo extends JdoDriver implements BarcelonaDriver{
    
    public void write(){
        int count = setup().getObjectCount();
        begin();
        for (int i = 1; i<= count; i++) {
            JB4 b4 = new JB4();
            b4.setAll(i);
            store(b4);
        }
        commit();
    }
    
    public void read(){
        readExtent(JB4.class);
    }
    
    public void query(){
        int count = setup().getSelectCount();
        String filter = "this.b2 == param";
        for (int i = 1; i <= count; i++) {
            Query query = db().newQuery(JB4.class, filter);
            query.declareParameters("int param");
            doQuery(query, i);
        }
    }
    
    public void delete(){
        begin();
        Extent extent = db().getExtent(JB4.class, false);
        Iterator it = extent.iterator();
        while(it.hasNext()){
            db().deletePersistent(it.next());
            addToCheckSum(5);
        }
        extent.closeAll();
        commit();
    }

}
