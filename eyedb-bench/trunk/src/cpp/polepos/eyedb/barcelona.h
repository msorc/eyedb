#ifndef _BARCELONA_H_
#define _BARCELONA_H_

#include "benchmark.h"

class Barcelona : public Benchmark {

public:
  Barcelona()
  {
  }

  void write();
  void read();
  void query();
  void destroy();

private:

};

#endif
