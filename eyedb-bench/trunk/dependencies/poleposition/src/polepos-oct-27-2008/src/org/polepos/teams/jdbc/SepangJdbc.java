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



import java.sql.ResultSet;
import java.sql.SQLException;

import org.polepos.circuits.sepang.*;
import org.polepos.framework.*;

/**
 * @author Herkules
 */
public class SepangJdbc extends JdbcDriver implements SepangDriver{
    
    Tree lastRead;
    
	public void takeSeatIn(Car car, TurnSetup setup) throws CarMotorFailureException{
		
		super.takeSeatIn(car, setup);
		
		// Open database
		jdbcCar().openConnection();
        
		//
		// Create database structure
		//
		jdbcCar().dropTable( "malaysia" );
		jdbcCar().createTable( "malaysia", new String[]{ "id", "preceding", "subsequent", "name", "depth" }, new Class[]{Integer.TYPE,Integer.TYPE,Integer.TYPE,String.class, Integer.TYPE} );

		jdbcCar().close();
	}
	
	public void write(){
        
        Tree tree = Tree.createTree(setup().getTreeDepth());
        Tree.traverse(tree, new TreeVisitor() {
            public void visit(Tree tree) {
                StringBuffer s = new StringBuffer( "insert into malaysia (id, preceding, subsequent, name, depth ) values (" );
                s.append(tree.id);
                s.append(",");
                if(tree.preceding != null){
                    s.append(tree.preceding.id);
                }else{
                    s.append(0);
                }
                s.append(",");
                if(tree.subsequent != null){
                    s.append(tree.subsequent.id);
                }else{
                    s.append(0);
                }
                s.append(", '");
                s.append(tree.name);
                s.append("', ");
                s.append(tree.depth);
                s.append(")");
                jdbcCar().executeSQL(s.toString());
            }
        });
		jdbcCar().commit();
	}

	
	public void read(){
        try {
            lastRead = read(1);
            Tree.traverse(lastRead, new TreeVisitor() {
                public void visit(Tree tree) {
                    addToCheckSum(tree.getDepth());
                }
            });
        } catch (SQLException e) {
            e.printStackTrace();
        }
	}
    
    private Tree read(int id) throws SQLException {
        JdbcCar car = jdbcCar();
		ResultSet rs = null;
		int precedingID, subsequentID;
		Tree tree = null;
		try {
			rs = car.executeQuery("select * from malaysia where id=" + id);
			rs.next();
			tree = new Tree(rs.getInt(1), rs.getString(4), rs.getInt(5));
			precedingID = rs.getInt(2);
			subsequentID = rs.getInt(3);
		} finally {
			car.closeQuery(rs);
		}
		
        if(precedingID > 0){
            tree.preceding = read(precedingID);
        }
        if(subsequentID > 0){
            tree.subsequent = read(subsequentID);
        }
        return tree;
    }
    
    public void read_hot() {
        read();
    }   
	
	public void delete(){
		try{
			delete(1);
            jdbcCar().commit();
		}catch ( SQLException sqlex ){ 
            sqlex.printStackTrace(); 
        }
	}
    
    private void delete(int id) throws SQLException{
        JdbcCar car = jdbcCar();
		ResultSet rs = null;
		int precedingID, subsequentID;
		try {
			rs = car.executeQuery("select * from malaysia where id=" + id);
			rs.next();
			precedingID = rs.getInt(2);
			subsequentID = rs.getInt(3);
		} finally {
			car.closeQuery(rs);
		}
        if(precedingID > 0){
            delete(precedingID);
        }
        if(subsequentID > 0){
            delete(subsequentID);
        }
        car.executeUpdate("delete from malaysia where id=" + id);
    }

}
