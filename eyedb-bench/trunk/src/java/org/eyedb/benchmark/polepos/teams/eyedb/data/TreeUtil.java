package org.eyedb.benchmark.polepos.teams.eyedb.data;

public class TreeUtil {
    public static Tree createTree( Database database, int depth) throws org.eyedb.Exception
    {
	return createTree( database, depth, 0);
    }

    private static Tree createTree( Database database, int maxDepth, int currentDepth) throws org.eyedb.Exception
    {
	if (currentDepth >= maxDepth)
	    return null;

	Tree tree = new Tree( database);

	if (currentDepth == 0)
	    tree.setName( "root");
	else
	    tree.setName( "node at depth " + currentDepth);

	tree.setDepth( currentDepth);

	tree.setPreceding( createTree( database, maxDepth, currentDepth + 1));
	tree.setSubsequent( createTree( database, maxDepth, currentDepth + 1));

	return tree;
    }

    public static void traverse( Tree tree, TreeVisitor visitor) throws org.eyedb.Exception
    {
        if (tree == null)
            return;

        traverse( tree.getPreceding(), visitor);
        traverse( tree.getSubsequent(), visitor);
        visitor.visit(tree);
    }
}
