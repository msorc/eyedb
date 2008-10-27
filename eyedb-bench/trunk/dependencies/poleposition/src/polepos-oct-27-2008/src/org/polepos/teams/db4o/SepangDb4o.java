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

import org.polepos.circuits.sepang.*;

/**
 * @author Herkules
 */
public class SepangDb4o extends Db4oDriver implements SepangDriver{
    
    long treeRootID;
    Tree lastRead;
    
	public void write(){
        begin();
        Tree tree = Tree.createTree(setup().getTreeDepth());
        store(tree);
        treeRootID = db().getID(tree);
		commit();
	}

	public void read(){
        lastRead = readAndActivate();
        Tree.traverse(lastRead, new TreeVisitor() {
            public void visit(Tree tree) {
                addToCheckSum(tree.getDepth());
            }
        });
	}

    public void read_hot() {
        read();
    }

    public void delete() {
        begin();
        Tree tree = readAndActivate();
        Tree.traverse(tree, new TreeVisitor() {
            public void visit(Tree tree) {
                db().delete(tree);
            }
        });
        commit();
    }
    
    private Tree readAndActivate(){
        Tree tree = (Tree)db().getByID(treeRootID);
        db().activate(tree, Integer.MAX_VALUE);
        return tree;
    }
}
