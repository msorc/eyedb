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

import org.polepos.framework.*;


/**
 * @author Herkules
 */
public abstract class JdoDriver extends Driver{
    
	private transient PersistenceManager mPersistenceManager;
    
	public void takeSeatIn( Car car, TurnSetup setup) throws CarMotorFailureException{
        super.takeSeatIn(car, setup);
        prepare();
	}
    
	public void prepare(){
		mPersistenceManager = jdoCar().getPersistenceManager();
	}
	
	public void backToPit(){
        Transaction tx = db().currentTransaction();
        if(tx.isActive()){
            tx.commit();
        }
		mPersistenceManager.close();
	}
	
	protected JdoCar jdoCar(){
		return (JdoCar)car();
	}
	
	protected PersistenceManager db(){
		return mPersistenceManager;
	}
    
    public void begin(){
        db().currentTransaction().begin();
    }
    
    public void commit(){
        db().currentTransaction().commit();
    }
    
    public void store(Object obj){
        db().makePersistent(obj);
    }
    
    protected void doQuery( Query q, Object param){
        Collection result = (Collection)q.execute(param);
        Iterator it = result.iterator();
        while(it.hasNext()){
            Object o = it.next();
            if(o instanceof CheckSummable){
                addToCheckSum(((CheckSummable)o).checkSum());
            }
        }
    }
    
    protected void readExtent(Class clazz){
        Extent extent = db().getExtent( clazz, false );
        Iterator itr = extent.iterator();
        while (itr.hasNext()){
            Object o = itr.next();
            if(o instanceof CheckSummable){
                addToCheckSum(((CheckSummable)o).checkSum());    
            }
        }
        extent.closeAll();
    }
    
    

}
