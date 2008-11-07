#ifndef _BAHRAIN_H_
#define _BAHRAIN_H_

#include "polepos-benchmark.h"

class Imola : public PoleposBenchmark {

public:

  void store( int count, int commitInterval);
  void retrieve( int selectCount);

  void run();

  const char* getName() const { return "Imola"; }
  const char* getDescription() const { return "write, query, update and delete simple flat objects individually"; }

 private:
  void query( int selectCount, const char *queryString);
};

#endif
