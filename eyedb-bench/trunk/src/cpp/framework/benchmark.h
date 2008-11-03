#ifndef _EYEDB_BENCHMARK_BENCHMARK_
#define _EYEDB_BENCHMARK_BENCHMARK_

#include "stopwatch.h"
#include "properties.h"
#include "result.h"
#include "reporter.h"

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
      StopWatch &getStopwatch() { return stopwatch; }

      const Properties &getProperties() const { return properties; }
      Properties &getProperties() { return properties; }

      const Result &getResult() const { return result; }
      Result &getResult() { return result; }

    private:
      StopWatch stopwatch;
      Properties properties;
      Result result;
    };
  };
};

#endif
