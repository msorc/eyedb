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

package org.polepos.teams.hibernate;



import java.util.Iterator;

import org.polepos.circuits.bahrain.*;
import org.polepos.teams.hibernate.data.*;

import net.sf.hibernate.*;


/**
 * @author Herkules
 */
public class BahrainHibernate extends HibernateDriver implements BahrainDriver{
     
    private final String FROM = "from org.polepos.teams.hibernate.data.HibernateIndexedPilot";
	
    public void write(){
        
        try{
            
            Transaction tx = db().beginTransaction();
    
            int commitctr = 0;
            int count = setup().getObjectCount();
            int commitInterval = setup().getCommitInterval();
            
            for ( int i = 1; i <= count; i++ ){
                
                db().save( new HibernateIndexedPilot( "Pilot_" + i, "Jonny_" + i, i , i ) );
                if ( commitInterval> 0  &&  ++commitctr >= commitInterval ){
                    commitctr = 0;
                    tx.commit();
                    Log.logger.fine( "commit while writing at " + i+1 ); //NOI18N
                }
                
                addToCheckSum(i);
                
            }
            
            tx.commit();
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
            
    }

    
    public void query_indexed_string(){
        
        int count = setup().getSelectCount();
        
        for (int i = 1; i <= count; i++) {
            doSingleResultQuery( FROM + " where Name='Pilot_" + i + "'" );
        }
        
    }

    
    public void query_string(){
        
        int count = setup().getSelectCount();
        
        for (int i = 1; i <= count; i++) {
            doSingleResultQuery( FROM + " where FirstName='Jonny_" + i + "'" );
        }
        
    }
	

    public void query_indexed_int(){
        
        int count = setup().getSelectCount();
        
        for (int i = 1; i <= count; i++) {
            doSingleResultQuery( FROM + " where LicenseID=" + i );
        }
        
    }

    public void query_int(){
        
        int count = setup().getSelectCount();
        
        for (int i = 1; i <= count; i++) {
            doQuery( FROM + " where Points=" + i );
        }
        
    }
    
    public void update(){
		
        try{
            int updateCount = setup().getUpdateCount();
            
			Transaction tx = db().beginTransaction();
			Iterator it = db().iterate(FROM);
            for (int i = 0; i < updateCount; i++) {
				HibernateIndexedPilot p = (HibernateIndexedPilot) it.next();
				p.setName( p.getName().toUpperCase() );
				db().update( p );
                addToCheckSum(1);
			}
			tx.commit();
        }
		catch ( HibernateException hex ){
			hex.printStackTrace();
		}
	}
    
    public void delete(){
        try{
            Transaction tx = db().beginTransaction();
            Iterator it = db().iterate(FROM);
            while(it.hasNext()){
                db().delete(it.next());
                addToCheckSum(1);
            }
            tx.commit();
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
    }
	
}
