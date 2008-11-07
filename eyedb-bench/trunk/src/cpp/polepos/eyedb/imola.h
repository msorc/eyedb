#ifndef _IMOLA_H_
#define _IMOLA_H_

#include <vector>
#include "polepos-benchmark.h"

class Imola : public PoleposBenchmark {

public:

  void store( int objectCount, int selectCount, int commitInterval);
  void retrieve();

  void run();

  const char* getName() const { return "Imola"; }
  const char* getDescription() const { return "retrieves objects by native id"; }

private:
  void storePilot( int i, int selectCount, int commitInterval) throw( eyedb::Exception);
  bool isCommitPoint( int i, int commitInterval);

  std::vector<eyedb::Oid> oids;
};

#endif
