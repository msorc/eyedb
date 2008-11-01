#ifndef _BARCELONA_H_
#define _BARCELONA_H_

#include "polepos-benchmark.h"

class Barcelona : public PoleposBenchmark {

public:

  void write( int count);
  void read();
  void query( int selectCount);
  void destroy();

  void run();

  const char* getName() { return "Barcelona"; }
  const char* getDescription() { return "writes, reads, queries and deletes objects with a 5 level inheritance structure"; }
};

#endif
