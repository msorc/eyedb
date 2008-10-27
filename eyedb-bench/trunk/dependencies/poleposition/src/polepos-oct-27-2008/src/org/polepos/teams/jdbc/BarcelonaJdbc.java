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

import org.polepos.circuits.barcelona.*;
import org.polepos.framework.*;



public class BarcelonaJdbc extends JdbcDriver implements BarcelonaDriver {
    
    private static final String[] TABLES = new String[]{
        "barcelona0",
        "barcelona1",
        "barcelona2",
        "barcelona3",
        "barcelona4",
    };
	private boolean _executeBatch;
    
    public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{
        
        super.takeSeatIn(car, setup);
        _executeBatch = jdbcCar().executeBatch();
        jdbcCar().openConnection();
        
        int i = 0;
        for(String table : TABLES){
            jdbcCar().dropTable( table);
            jdbcCar().createTable( table, new String[]{ "id", "parent", "b" + i}, 
                        new Class[]{Integer.TYPE, Integer.TYPE, Integer.TYPE} );
            jdbcCar().createIndex( table, "parent" );
            if(i == 2){
                jdbcCar().createIndex( table, "b2" );
            }
            i++;
        }
        jdbcCar().close();

    }


    public void write() {
        
        try{
            PreparedStatement[] statements = new PreparedStatement[5];
            for (int i = 0; i < 5; i++) {
                statements[i] = jdbcCar().prepareStatement("insert into " + TABLES[i] + " (id, parent, b" + i + ") values (?,?,?)");
            }
            
            int count = setup().getObjectCount();
			for (int j = 0; j < 5; j++) {
				for (int i = 1; i <= count; i++) {
					B4 b4 = new B4();
					b4.setAll(i);
					statements[j].setInt(1, i);
					statements[j].setInt(2, i);
					statements[j].setInt(3, b4.getBx(j));
					if (_executeBatch) {
						statements[j].addBatch();
					} else {
						statements[j].execute();
					}
				}
				if (_executeBatch) {
					statements[j].executeBatch();
				}
				statements[j].close();
			}
		} catch (SQLException sqlex) {
			sqlex.printStackTrace();
		}
        
        jdbcCar().commit();     
        
    }


    public void read() {
        StringBuffer sql = select();
        sql.append(TABLES[0]);
        sql.append(".id=?");
        query(sql.toString(), setup().getObjectCount());
    }


    public void query() {
        StringBuffer sql = select();
        sql.append(TABLES[2]);
        sql.append(".b2=?");
        query(sql.toString(), setup().getSelectCount());
    }

    /**
     * deleting one at a time, simulating deleting individual objects  
     */
    public void delete(){
        
        int count = setup().getObjectCount();
        
        PreparedStatement[] statements = new PreparedStatement[5];
        for (int i = 0; i < 5; i++) {
            statements[i] = jdbcCar().prepareStatement("delete from " + TABLES[i] + " where id=?");
        }
        
        try {
			for (int j = 0; j < 5; j++) {
				for (int i = 1; i <= count; i++) {
					statements[j].setInt(1, i);
					addToCheckSum(1);
					if (_executeBatch) {
						statements[j].addBatch();
					} else {
						statements[j].execute();
					}
				}
				if(_executeBatch) {
					statements[j].executeBatch();
				}
				statements[j].close();
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
        
        jdbcCar().commit();     
    }
    
    private StringBuffer select(){
        StringBuffer sql = new StringBuffer("select * from ");
        sql.append(TABLES[0]);
        for (int i = 1; i < TABLES.length; i++) {
            sql.append(", ");
            sql.append(TABLES[i]);
        }
        sql.append(" where ");
        for (int i = 1; i < TABLES.length; i++) {
            sql.append(TABLES[i]);
            sql.append(".parent = ");
            sql.append(TABLES[i - 1]);
            sql.append(".id");
            sql.append(" and ");
        }
        return sql;
    }
    
    private void query(String sql, int count){
        PreparedStatement statement = jdbcCar().prepareStatement(sql.toString());
        
        try {
            for(int i = 1 ; i <= count; i++) {
                statement.setInt(1,i);
                ResultSet rs=statement.executeQuery();
                if(!rs.next()) {
                    System.err.println("Expected one result, received none: "+ i);
                }
                B4 b4 = new B4(rs.getInt(3), rs.getInt(6), rs.getInt(9), rs.getInt(12), rs.getInt(15));
                addToCheckSum(b4.checkSum());
                if(rs.next()) {
                    System.err.println("Expected one result, received multiple: "+i);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
    }
    
   
    
    
    


}
