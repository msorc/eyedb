#include "stopwatch.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:
    Benchmark( const std::string &name) : name(name) {}

      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void lap( const std::string &name) { stopwatch.lap( name); }

      unsigned long measure();

    private:
      StopWatch stopwatch;
    };
  };
};
