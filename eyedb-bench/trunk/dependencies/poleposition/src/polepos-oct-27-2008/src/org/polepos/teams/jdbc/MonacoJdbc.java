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

import java.sql.*;

import org.polepos.circuits.monaco.*;
import org.polepos.framework.*;


public class MonacoJdbc extends JdbcDriver implements MonacoDriver{

    public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{
        
        super.takeSeatIn(car, setup);

        jdbcCar().openConnection();
        
        jdbcCar().dropTable( "monaco" );
        jdbcCar().createTable( "monaco", new String[]{ "id", "name"}, 
            new Class[]{Integer.TYPE, String.class} );
        
        jdbcCar().close();

    }
    
    public void write() {
        
        try{
            PreparedStatement statement = jdbcCar().prepareStatement("insert into monaco (id, name) values (?,?)");

            int commitctr = 0;
            int commitInterval = 50000;

            int count = setup().getObjectCount();
            
            if(count > 0){
                for (int i = 1; i <= count; i++) {
                    LightObject m1 = new LightObject(i);
                    statement.setInt(1, i);
                    statement.setString(2, m1.getName());
                    statement.execute();
                    
                    if ( commitInterval> 0  &&  ++commitctr >= commitInterval ){
                        commitctr = 0;
                        jdbcCar().commit();
                    }

                }
                
                statement.close();
            }
            
            jdbcCar().commit();     

        }catch ( SQLException sqlex ){
            sqlex.printStackTrace();
        }
        
    }
    
    public void commits(){
        try{
            PreparedStatement statement = jdbcCar().prepareStatement("insert into monaco (id, name) values (?,?)");

            int idbase = setup().getObjectCount() + 1;
            int count = setup().getCommitCount();
            
            if(count > 0){
                for (int i = 1; i <= count; i++) {
                    LightObject m1 = new LightObject(idbase + i);
                    statement.setInt(1, idbase + i);
                    statement.setString(2, m1.getName());
                    statement.execute();
                    jdbcCar().commit();
                }
                
                statement.close();
            }

        }catch ( SQLException sqlex ){
            sqlex.printStackTrace();
        }

        
        
    
    }

}
