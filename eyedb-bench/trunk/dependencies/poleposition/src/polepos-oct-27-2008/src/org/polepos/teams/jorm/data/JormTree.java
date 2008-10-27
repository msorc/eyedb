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

package org.polepos.teams.jorm.data;

import de.ama.db.PersistentMarker;


public class JormTree implements PersistentMarker{
    
    private static long idGenerator;
    
    private long Id;
    private JormTree preceding;
    private JormTree subsequent;
    private String name;
    private int depth;
    
    public JormTree(){
    }


    public void setDepth(int depth) {
        this.depth = depth;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public JormTree getPreceding() {
        return preceding;
    }

    public void setPreceding(JormTree preceding) {
        this.preceding = preceding;
    }

    public JormTree getSubsequent() {
        return subsequent;
    }

    public void setSubsequent(JormTree subsequent) {
        this.subsequent = subsequent;
    }

    public long getId() {
        return Id;
    }

    public void setId(long id) {
        this.Id = id;
    }

    public JormTree(long id, String name, int depth){
        this.Id = id;
        this.name = name;
        this.depth = depth;
    }
    
    public static JormTree createTree(int depth){
        idGenerator = 0;
        return createTree(depth, 0);
    }
    
    private static JormTree createTree(int maxDepth, int currentDepth){
        
        if(maxDepth <= 0){
            return null;
        }
        
        JormTree tree = new JormTree();
        if(currentDepth == 0){
            tree.name = "root";
        }else{
            tree.name = "node at depth " + currentDepth;
        }
        tree.setId(++idGenerator);
        tree.setDepth(currentDepth);
        tree.setPreceding(createTree(maxDepth - 1, currentDepth + 1));
        tree.setSubsequent(createTree(maxDepth - 1, currentDepth + 1));
        return tree;
    }
    
    public static void traverse(JormTree tree, JormTreeVisitor visitor){
        if(tree == null){
            return;
        }
        traverse(tree.getPreceding(), visitor);
        traverse(tree.getSubsequent(), visitor);
        visitor.visit(tree);
    }
    
    public int getDepth(){
        return depth;
    }
    
    

}
