#include <sstream>
#include <vector>
#include "polepos.h"
#include "sepang.h"
#include "treeutil.h"

using namespace std;

Tree *Sepang::findRoot() throw( eyedb::Exception)
{
  eyedb::OQL q(getDatabase(), "select t from Tree as t where t.name = \"root\"");
  eyedb::ObjectArray arr;

  q.execute(arr);

  return Tree_c( arr[0]);
}

void Sepang::write( int depth)
{
  try {
    getDatabase()->transactionBegin();

    Tree *tree = TreeUtil::createTree( getDatabase(), depth);

    tree->store( eyedb::FullRecurs);
                
    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

class CheckSumTreeVisitor : public TreeVisitor {
public:
  CheckSumTreeVisitor() : sum( 0) {}
  void visit( Tree * tree);
private:
  int sum;
};

void CheckSumTreeVisitor::visit( Tree * tree)
{
  try {
    sum += tree->getDepth();
  } catch (eyedb::Exception &ex) {
    ex.print();
  }
}

void Sepang::read()
{
  try {
    getDatabase()->transactionBegin();

    CheckSumTreeVisitor visitor;
    TreeUtil::traverse( findRoot(), visitor);

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Sepang::read_hot()
{
  read();
}

class RemoveTreeVisitor : public TreeVisitor {
public:
  void visit( Tree * tree);
};

void RemoveTreeVisitor::visit( Tree * tree)
{
  try {
    tree->remove();
  } catch (eyedb::Exception &ex) {
    ex.print();
  }
}

void Sepang::remove()
{
  try {
    getDatabase()->transactionBegin();

    RemoveTreeVisitor visitor;
    TreeUtil::traverse( findRoot(), visitor);

    getDatabase()->transactionCommit();
  }
  catch ( eyedb::Exception &ex ) {
    ex.print();
  }
}

void Sepang::run()
{
  vector<int> depths;

  getProperties().getIntProperty( "sepang.depth", depths);
  
  getResult().addHeader( "depth");

  for (int i = 0; i < depths.size(); i++) {

    getResult().add( depths[i]);

    getStopwatch().start();

    write( depths[i]);
    getStopwatch().lap( "write");

    read();
    getStopwatch().lap( "read");
    read_hot();
    getStopwatch().lap( "read_hot");

    remove();
    getStopwatch().lap( "remove");

    getStopwatch().stop();

    getResult().add( getStopwatch().getLaps());

    getStopwatch().reset();

    getResult().next();
  }
}

int main(int argc, char *argv[])
{
  Sepang b;
  b.getProperties().load( "eyedb.properties");
  b.getProperties().load( argc, argv);

  polepos initializer(argc, argv);

  b.bench();

  eyedb::benchmark::SimpleReporter r;
  r.setColumnWidth( 16);
  r.report(b);
}
