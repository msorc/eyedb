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

import java.util.*;

import org.polepos.circuits.barcelona.*;
import org.polepos.teams.hibernate.data.*;

import net.sf.hibernate.*;




public class BarcelonaHibernate extends HibernateDriver implements BarcelonaDriver{
    
    private final String FROM = "from org.polepos.teams.hibernate.data.HB4";

    public void write(){
        try{
            Transaction tx = db().beginTransaction();
            
            int count = setup().getObjectCount(); 
            for (int i = 1; i<= count; i++) {
                HB4 b4 = new HB4();
                b4.setAll(i);
                db().save(b4);
            }
            
            tx.commit();
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
    }
    
    public void read(){
        doQuery(FROM);
    }
    
    public void query(){
        int count = setup().getSelectCount();
        for (int i = 1; i <= count; i++) {
            doQuery(FROM + " where b2=" + i);
        }
    }
    
    public void delete(){
        try{
            Transaction tx = db().beginTransaction();
            Iterator it = db().iterate(FROM);
            while(it.hasNext()){
                db().delete(it.next());
                addToCheckSum(5);
            }
            tx.commit();
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
    }

}
