#include <iostream>
#include <map>
#include "stopwatch.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:

      Benchmark();

      virtual const char* getName() = 0;
      virtual const char* getDescription() = 0;
      virtual const char* getRunDescription() = 0;
      
      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void bench();

      void setColumnWidth( int width) { columnWidth = width; }
      void setColumnSeparator( char separator) { columnSeparator = separator; }
      void addColumnHeader( const std::string &header) { columnHeaders.push_back( header); }
      void addRowHeader( const std::string &header) { rowHeaders.push_back( header); }
      void reportBegin();
      void reportLaps();
      void reportEnd();

      StopWatch &getStopwatch() { return stopwatch; }

      void loadProperties( const std::string &filename);
      void loadProperties( std::istream &is);
      void loadProperties( int &argc, char **argv);

      int getIntProperty( const std::string &name, int &value, int defaultValue = -1);
      int getIntProperty( const std::string &name, std::vector<int> &values);

      int getStringProperty( const std::string &name, std::string &value, const std::string &defaultValue = "");

      void printProperties();

    private:
      void lexicalError( char c, int pos, int line);

      StopWatch stopwatch;
      std::map<const std::string, std::string> properties;

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
