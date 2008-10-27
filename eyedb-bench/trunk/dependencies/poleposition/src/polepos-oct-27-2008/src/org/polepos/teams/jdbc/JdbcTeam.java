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

package org.polepos.teams.jdbc;

import org.polepos.framework.*;


public class JdbcTeam extends Team{
    
	private final Car[] mCars;
	
	public JdbcTeam(){
        
		String[] dbs = Jdbc.settings().getJdbcTypes();
		mCars = new Car[ dbs.length ];
		
		for( int i = 0; i < dbs.length; i++ ){
			try {
                mCars[i] = new JdbcCar(dbs[ i ] );
            } catch (CarMotorFailureException e) {
                mCars[i] = null;
            } 
		}
	}
	
    @Override
	public String name(){
		return "JDBC";
	}
    
    @Override
    public String description() {
        return "all JDBC databases registered in Jdbc.properties";
    }

    @Override
	public Car[] cars(){
		return mCars;
	}

    @Override
    public Driver[] drivers() {
        return new Driver[]{
            new MelbourneJdbc(),
            new SepangJdbc(),
            new BahrainJdbc(),
            new ImolaJdbc(),
            new BarcelonaJdbc(),
            new MonacoJdbc()
        };
    }
    
    @Override
    public String website() {
        return null;
    }

    public String databaseFile() {
    	// not supported yet
    	return null;
    }
    
    @Override
    public void configure(int[] options) {
        // TODO Auto-generated method stub
    }


}
