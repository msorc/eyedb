#include <sstream>
#include "treeutil.h"

#include <iostream>

using namespace std;

Tree * TreeUtil::createTree( eyedb::Database *database, int depth) throw( eyedb::Exception)
{
  return createTree( database, depth, 0);
}

void TreeUtil::traverse( Tree *tree, TreeVisitor &visitor) throw( eyedb::Exception)
{
  if (tree == 0)
    return;

  traverse( tree->getPreceding(), visitor);
  traverse( tree->getSubsequent(), visitor);
  visitor.visit(tree);
}

Tree * TreeUtil::createTree( eyedb::Database *database, int maxDepth, int currentDepth) throw( eyedb::Exception)
{
  if (currentDepth >= maxDepth)
    return 0;
      
  Tree *tree = new Tree( database);

  if (currentDepth == 0)
    tree->setName( "root");
  else {
    ostringstream oss;

    oss << "node at depth " <<  currentDepth;
    
    tree->setName( oss.str());
  }

  tree->setDepth( currentDepth);

  tree->setPreceding( createTree( database, maxDepth, currentDepth + 1));
  tree->setSubsequent( createTree( database, maxDepth, currentDepth + 1));

  return tree;
}

class PrintTreeVisitor : public TreeVisitor {
public:
  void visit( Tree * tree);
};

void PrintTreeVisitor::visit( Tree * tree)
{
  try {
    cout << "Tree: " << tree->getDepth() << " \"" << tree->getName() << "\"" << endl;
  } catch (eyedb::Exception &ex) {
    ex.print();
  }
}

void TreeUtil::print( Tree *tree)
{
    PrintTreeVisitor visitor;
    TreeUtil::traverse( tree, visitor);
}

