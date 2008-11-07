#ifndef _SEPANG_H_
#define _SEPANG_H_

#include "polepos-benchmark.h"

class Sepang : public PoleposBenchmark {

public:

  void write( int depth);
  void read();
  void read_hot();
  void remove();

  void run();

  const char* getName() const { return "Sepang"; }
  const char* getDescription() const { return "writes, reads and then deletes an object tree"; }

 private:
  Tree *findRoot() throw( eyedb::Exception);
};

#endif
