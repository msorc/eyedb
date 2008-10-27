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

import org.polepos.circuits.sepang.*;
import org.polepos.teams.hibernate.data.*;

import net.sf.hibernate.*;

/**
 * @author Herkules
 */
public class SepangHibernate extends HibernateDriver implements SepangDriver{
    
    private long treeRootID; // TODO: get and use for findRoot
    
    public void write(){
        try{
            Transaction tx = db().beginTransaction();
            HibernateTree tree = HibernateTree.createTree(setup().getTreeDepth());
            HibernateTree.traverse(tree, new HibernateTreeVisitor() {
                public void visit(HibernateTree tree) {
                    try {
                        db().save(tree);
                    } catch (HibernateException e) {
                        e.printStackTrace();
                    }
                }
            });
            tx.commit();
            
        }
        catch ( HibernateException hex ){
            hex.printStackTrace();
        }
    }

	
	public void read(){
		try{
            HibernateTree tree = findRoot();
            HibernateTree.traverse(tree, new HibernateTreeVisitor() {
                public void visit(HibernateTree tree) {
                    addToCheckSum(tree.getDepth());
                }
            });
		}catch ( HibernateException hex ){
			hex.printStackTrace();
		}		
	}
    
    public void read_hot() {
        read();
    }
	
	public void delete(){
		try{
			Transaction tx = db().beginTransaction();
            HibernateTree tree = findRoot();
            HibernateTree.traverse(tree, new HibernateTreeVisitor() {
                public void visit(HibernateTree tree) {
                    try {
                        db().delete(tree);
                    } catch (HibernateException e) {
                        e.printStackTrace();
                    }
                }
            });
			tx.commit();
		}catch ( HibernateException hex ){
			hex.printStackTrace();
		}		
	}
	
	/**
	 * Finds the root of the tree.
	 */
	private HibernateTree findRoot() throws HibernateException
	{
		Query q = db().createQuery( "select tree from tree in class org.polepos.teams.hibernate.data.HibernateTree where tree.name='root'" );
        HibernateTree tree = (HibernateTree)q.list().get(0);
		return tree;
	}
		
}
