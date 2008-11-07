#ifndef _BAHRAIN_H_
#define _BAHRAIN_H_

#include "polepos-benchmark.h"

class Bahrain : public PoleposBenchmark {

public:

  void write( int count, int commitInterval);
  void query_indexed_string( int selectCount);
  void query_string( int selectCount);
  void query_indexed_int( int selectCount);
  void query_int( int selectCount);
  void update( int updateCount);
  void remove();

  void run();

  const char* getName() const { return "Bahrain"; }
  const char* getDescription() const { return "write, query, update and delete simple flat objects individually"; }

 private:
  void query( int selectCount, const char *queryString);
};

#endif
