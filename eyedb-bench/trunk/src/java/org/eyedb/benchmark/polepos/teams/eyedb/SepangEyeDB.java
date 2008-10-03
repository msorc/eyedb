package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.Iterator;
import org.eyedb.benchmark.polepos.teams.eyedb.data.Tree;
import org.eyedb.benchmark.polepos.teams.eyedb.data.TreeUtil;
import org.eyedb.benchmark.polepos.teams.eyedb.data.TreeVisitor;
import org.polepos.circuits.sepang.SepangDriver;

public class SepangEyeDB extends EyeDBDriver implements SepangDriver {
    
    private Tree findRoot() throws org.eyedb.Exception
    {
	Tree tree = null;

	Iterator it = iterate( "select t from Tree as t where t.name = \"root\"");

	while (it.hasNext()) {
	    tree = (Tree)it.next();
	    break;
	}

	return tree;
    }

    public void write()
    {
	try {
	    getDatabase().transactionBegin();

	    int depth = setup().getTreeDepth();

	    Tree tree = TreeUtil.createTree( (org.eyedb.benchmark.polepos.teams.eyedb.data.Database)getDatabase(), depth);

	    tree.store( org.eyedb.RecMode.FullRecurs);

            getDatabase().transactionCommit();
	}
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
        }
    }
    
    public void read()
    {
	try {
	    getDatabase().transactionBegin();

            TreeUtil.traverse( findRoot(), new TreeVisitor() {
		    public void visit( Tree tree)
		    {
			try {
			    addToCheckSum( tree.getDepth());
			} catch (org.eyedb.Exception ex) {
			    ex.printStackTrace();
			}
		    }
		});

            getDatabase().transactionCommit();
	}
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
	}		
    }
    
    public void read_hot()
    {
        read();
    }
    
    public void delete()
    {
	try {
	    getDatabase().transactionBegin();

            TreeUtil.traverse( findRoot(), new TreeVisitor() {
		    public void visit( Tree tree) 
		    {
			try {
			    tree.remove();
			} catch (org.eyedb.Exception ex) {
			    ex.printStackTrace();
			}
		    }
		});

            getDatabase().transactionCommit();
	}
        catch ( org.eyedb.Exception ex ) {
            ex.printStackTrace();
	}		
    }
}
