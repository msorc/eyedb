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

import org.polepos.framework.*;


public class JdoTeam extends Team{
    
	private final Car[] mCars;
    
    public JdoTeam() {
        
        String[] impls = Jdo.settings().getJdoImplementations();
        
        if(impls == null){
            System.out.println("No JDO engine configured.");
            mCars = new Car[0];
        }else{
        
            List <Car> cars = new ArrayList<Car>();
            
            for (String impl : impls) {
                
                String[] jdosqldbs = Jdo.settings().getJdbc(impl);
                
                if(jdosqldbs != null && jdosqldbs.length > 0){
                    for(String sqldb : jdosqldbs){
                        try {
                            cars.add(new JdoCar(impl, sqldb));
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }else{
                    try {
                        cars.add(new JdoCar(impl, null));
                    } catch (Exception e) {
                        e.printStackTrace();
                    } 
                }
            }
            
            mCars = new Car[ cars.size() ];
            cars.toArray(mCars);
        }
        
    }
    
	
    @Override
	public String name(){
		return "JDO";
	}

    @Override
    public String description() {
        return "the JDO team";
    }

    @Override
	public Car[] cars(){
		return mCars;
	}
    
    public String databaseFile() {
    	// not supported yet
    	return null;
    }

    @Override
    public Driver[] drivers() {
        return new Driver[]{
            new MelbourneJdo(),
            new SepangJdo(),
            new BahrainJdo(),
            new ImolaJdo(),
            new BarcelonaJdo()
        };
    }
    
    @Override
    public String website() {
        return null;
    }


    @Override
    public void configure(int[] options) {
        // TODO Auto-generated method stub
        
    }


	
}
