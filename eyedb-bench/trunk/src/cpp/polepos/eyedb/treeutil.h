#ifndef _TREEUTIL_H_
#define _TREEUTIL_H_

#include "polepos.h"

class TreeVisitor {
 public:
  virtual void visit( Tree *tree) = 0;
};

class TreeUtil {
 public:
  static Tree * createTree( eyedb::Database *database, int depth) throw( eyedb::Exception);

  static void traverse( Tree *tree, TreeVisitor &visitor) throw( eyedb::Exception);

  static void print( Tree * tree);

 private:
  static Tree * createTree( eyedb::Database *database, int maxDepth, int currentDepth) throw( eyedb::Exception);
};

#endif
