#ifndef _EYEDB_BENCHMARK_REPORTER_
#define _EYEDB_BENCHMARK_REPORTER_

#include <iostream>
#include <fstream>
#include <vector>
#include "benchmark.h"
#include "properties.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark;

    class Reporter {
    public:
      virtual void report( const Benchmark & benchmark)  = 0;
    };

    class DefaultReporter : public Reporter {
    public:
      DefaultReporter();

      virtual void report( const Benchmark &benchmark);

    private:
      std::vector<Reporter *> reporters;
    };


    class SimpleReporter : public Reporter {
    public:
      SimpleReporter() : columnWidth( defaultColumnWidth), columnSeparator( defaultColumnSeparator) {}

      virtual void report( const Benchmark &benchmark);

      void setColumnWidth( int width) { columnWidth = width; }
      void setColumnSeparator( char separator) { columnSeparator = separator; }

    private:
      void reportResult( const Result &result);

      int columnWidth;
      char columnSeparator;

      static const int defaultColumnWidth;
      static const char defaultColumnSeparator;
    };

    class CSVReporter : public Reporter {
    public:
      CSVReporter() : columnSeparator( defaultColumnSeparator) {}

      virtual void report( const Benchmark &benchmark);

      void setColumnSeparator( char separator) { columnSeparator = separator; }
    private:
      void reportResult( const Result &result);

      char columnSeparator;
      std::ofstream outfile;

      static const char defaultColumnSeparator;

    };

  };
};

#endif
