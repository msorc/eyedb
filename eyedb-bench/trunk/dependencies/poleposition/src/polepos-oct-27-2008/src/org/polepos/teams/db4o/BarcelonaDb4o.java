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

import org.polepos.circuits.barcelona.*;
import org.polepos.framework.*;

import com.db4o.*;
import com.db4o.query.*;


public class BarcelonaDb4o extends Db4oDriver implements BarcelonaDriver{
    
    public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{
        Db4o.configure().objectClass( B2.class ).objectField( "b2" ).indexed( true );
        super.takeSeatIn(car, setup);
    }
    
    public void write(){
        int count = setup().getObjectCount();
        begin();
        for (int i = 1; i<= count; i++) {
            B4 b4 = new B4();
            b4.setAll(i);
            store(b4);
        }
        commit();
    }
    
    public void read(){
        doQuery(B4.class);
    }
    
    public void query(){
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            Query q = db().query();
            q.constrain(B4.class);
            q.descend("b2").constrain(i);
            doQuery(q);
        }
    }
    
    public void delete(){
        begin();
        Query q = db().query();
        q.constrain(B4.class);
        ObjectSet deleteSet = q.execute();
        while(deleteSet.hasNext()){
            db().delete(deleteSet.next());
            addToCheckSum(5);
        }
        commit();
    }

}
