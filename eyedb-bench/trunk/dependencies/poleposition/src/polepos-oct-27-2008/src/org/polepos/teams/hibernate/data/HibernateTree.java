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

package org.polepos.teams.hibernate.data;


public class HibernateTree {
    
    public long id;
    public HibernateTree preceding;
    public HibernateTree subsequent;
    public String name;
    public int depth;
    
    public HibernateTree(){
    }
    
    public HibernateTree(String name, int depth){
        this.name = name;
        this.depth = depth;
    }
    
    public long getId(){
        return id;
    }
    
    public void setId(long id){
        this.id = id;;
    }

    public HibernateTree getPreceding(){
        return preceding;
    }
    
    public void setPreceding(HibernateTree tree){
        preceding = tree;
    }

    public HibernateTree getSubsequent(){
        return subsequent;
    }
    
    public void setSubsequent(HibernateTree tree){
        subsequent = tree;
    }
    
    public String getName(){
        return name;
    }
    
    public void setName(String name){
        this.name = name;
    }
    
    public int getDepth(){
        return depth;
    }
    
    public void setDepth(int depth){
        this.depth = depth;
    }
    
    public static HibernateTree createTree(int depth){
        return createTree(depth, 0);
    }
    
    private static HibernateTree createTree(int maxDepth, int currentDepth){
        
        if(maxDepth <= 0){
            return null;
        }
        
        HibernateTree tree = new HibernateTree();
        if(currentDepth == 0){
            tree.name = "root";
        }else{
            tree.name = "node at depth " + currentDepth;
        }
        tree.depth =  currentDepth;
        tree.preceding = createTree(maxDepth - 1, currentDepth + 1); 
        tree.subsequent = createTree(maxDepth - 1, currentDepth + 1); 
        return tree;
    }
    
    public static void traverse(HibernateTree tree, HibernateTreeVisitor visitor){
        if(tree == null){
            return;
        }
        traverse(tree.preceding, visitor);
        traverse(tree.subsequent, visitor);
        visitor.visit(tree);
    }

}
