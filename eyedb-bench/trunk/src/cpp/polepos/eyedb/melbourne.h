#ifndef _MELBOURNE_H_
#define _MELBOURNE_H_

#include "polepos-benchmark.h"

class Melbourne : public PoleposBenchmark {

public:

  void write( int objectCount, int commitInterval);
  void read( int selectCount);
  void read_hot( int selectCount);
  void remove();

  void run();

  const char* getName() const { return "Melbourne"; }
  const char* getDescription() const { return "writes, reads and deletes unstructured flat objects of one kind in bulk mode"; }
};

#endif
