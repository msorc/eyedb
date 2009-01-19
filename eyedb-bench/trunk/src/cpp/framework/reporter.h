#ifndef _EYEDB_BENCHMARK_REPORTER_
#define _EYEDB_BENCHMARK_REPORTER_

#include "benchmark.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark;

    class Reporter {
    public:
      virtual void report( const Benchmark & benchmark)  = 0;
    };

    class SimpleReporter : public Reporter {
    public:
      SimpleReporter();

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
      CSVReporter();

      virtual void report( const Benchmark &benchmark);

      void setColumnSeparator( char separator) { columnSeparator = separator; }
    private:
      char columnSeparator;

      static const char defaultColumnSeparator;
    };

  };
};

#endif
