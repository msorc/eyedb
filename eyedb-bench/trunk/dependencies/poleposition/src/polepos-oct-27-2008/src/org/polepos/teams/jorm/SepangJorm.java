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

package org.polepos.teams.jorm;
import org.polepos.circuits.sepang.SepangDriver;
import org.polepos.teams.jorm.data.JormTree;
import org.polepos.teams.jorm.data.JormTreeVisitor;


/**
 * @author Andreas Marochow
 */
public class SepangJorm extends JormDriver implements SepangDriver {
    
    private Object oid;
    
	public void write(){

        JormTree tree = JormTree.createTree(setup().getTreeDepth());
        db().setObject(tree);
        oid = db().getOidString(tree);
		commit();
	}

	public void read(){
        JormTree tree = (JormTree)db().getObject((String) oid);
        JormTree.traverse(tree, new JormTreeVisitor() {
            public void visit(JormTree tree) {
                addToCheckSum(tree.getDepth());
            }
        });
	}
    
    public void read_hot() {
        read();
    }

	public void delete(){
        JormTree tree = (JormTree)db().getObject((String) oid);
        JormTree.traverse(tree, new JormTreeVisitor() {
            public void visit(JormTree tree) {
                db().delete(tree);
            }
        });
		commit();
	}
    

}
