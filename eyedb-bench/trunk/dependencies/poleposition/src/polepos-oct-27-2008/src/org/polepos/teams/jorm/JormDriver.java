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

import de.ama.db.DB;
import de.ama.db.Session;
import org.polepos.framework.Car;
import org.polepos.framework.CarMotorFailureException;
import org.polepos.framework.Driver;
import org.polepos.framework.TurnSetup;


/**
 * @author Andreas Marochow
 */
public abstract class JormDriver extends Driver{
    

	public void takeSeatIn( Car car, TurnSetup setup) throws CarMotorFailureException{
        super.takeSeatIn(car, setup);
        prepare();
	}
    
	public void prepare(){
//        DB.session().delete(Persistent.class);
	}
	
	public void backToPit(){
        DB.session().rollback();
	}

    public Session db(){
        return DB.session();
    }

    public void commit(){
        DB.session().commit();
    }
    
    public void store(Object obj){
        DB.session().setObject(obj);
    }
    

    
    

}
