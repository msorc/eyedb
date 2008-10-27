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

import org.polepos.circuits.monaco.*;

public class MonacoDb4o extends Db4oDriver implements MonacoDriver{

    public void write() {
        
        int commitctr = 0;
        int commitInterval = 50000;

        int count = setup().getObjectCount();
        
        begin();
        for (int i = 1; i<= count; i++) {
            store(new LightObject(i));
            if ( commitInterval> 0  &&  ++commitctr >= commitInterval ){
                commitctr = 0;
                commit();
            }
        }
        commit();
    }
    
    public void commits(){
        
        int idbase = setup().getObjectCount() + 1;
        int count = setup().getCommitCount();
        
        begin();
        for (int i = 1; i<= count; i++) {
            store(new LightObject(idbase + i));
            commit();
            begin();
        }
        
    }

}
