#ifndef _EYEDB_BENCHMARK_BENCHMARK_
#define _EYEDB_BENCHMARK_BENCHMARK_

#include "stopwatch.h"
#include "properties.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:

      virtual const char* getName() const = 0;
      virtual const char* getDescription() const = 0;
      virtual const char* getRunDescription() const = 0;
      
      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void bench();

      const StopWatch &getStopwatch() const { return stopwatch; }

      Properties &getProperties() { return properties; }

    private:
      StopWatch stopwatch;
      Properties properties;
    };
  };
};

#endif
