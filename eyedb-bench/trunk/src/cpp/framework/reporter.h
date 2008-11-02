#ifndef _EYEDB_BENCHMARK_REPORTER_
#define _EYEDB_BENCHMARK_REPORTER_

#include <string>
#include <vector>
#include "benchmark.h"

namespace eyedb {
  namespace benchmark {
    class Reporter {
    public:
      Reporter();

      void setColumnWidth( int width) { columnWidth = width; }
      void setColumnSeparator( char separator) { columnSeparator = separator; }

      void addColumnHeader( const std::string &header) { columnHeaders.push_back( header); }
      void addRowHeader( const std::string &header) { rowHeaders.push_back( header); }

      virtual void reportBegin( const Benchmark &benchmark)/*  = 0 */;
      virtual void reportLaps( const Benchmark &benchmark)/*  = 0 */;
      virtual void reportEnd( const Benchmark &benchmark)/*  = 0 */;

    private:
      std::vector< std::string> columnHeaders;
      std::vector< std::string> rowHeaders;
      int columnWidth;
      char columnSeparator;

      bool reportLapsDone;
      bool reportColumnHeadersDone;

      static const int defaultColumnWidth;
      static const char defaultColumnSeparator;
    };
  };
};

#endif
