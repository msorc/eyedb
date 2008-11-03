#ifndef _BAHRAIN_H_
#define _BAHRAIN_H_

#include "polepos-benchmark.h"

class Bahrain : public PoleposBenchmark {

public:

  void write( int count);
  void query_indexed_string( int selectCount);
  void query_string( int selectCount);
  void query_indexed_int( int selectCount);
  void query_int( int selectCount);
  void update( int updateCount);
  void destroy();

  void run();

  const char* getName() const { return "Bahrain"; }
  const char* getDescription() const { return "write, query, update and delete simple flat objects individually"; }
};

#endif
