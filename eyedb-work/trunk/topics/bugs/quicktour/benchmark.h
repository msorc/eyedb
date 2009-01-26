#ifndef _EYEDB_BENCHMARK_BENCHMARK_
#define _EYEDB_BENCHMARK_BENCHMARK_

#include "properties.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:

      virtual const char* getName() const = 0;
      virtual const char* getDescription() const = 0;
      virtual const char* getImplementation() const = 0;
      
      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void bench();

      const Properties &getProperties() const { return properties; }
      Properties &getProperties() { return properties; }
      void setProperties( const Properties &properties) { this->properties = properties; }

    private:
      Properties properties;
    };
  };
};

#endif
