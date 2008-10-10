#include <iostream>
#include <map>
#include "stopwatch.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:

      void loadProperties( const std::string &filename);
      void loadProperties( std::istream &is);

      virtual const char* getName() = 0;
      virtual const char* getDescription() = 0;

      virtual void prepare() = 0;
      virtual void run() = 0;
      virtual void finish() = 0;

      void lap( const std::string &name) 
      { 
	stopwatch.lap( name); 
      }

      void bench();

      void printProperties();

    private:
      void lexicalError( char c);

      StopWatch stopwatch;
      std::map<const std::string, std::string> properties;
    };
  };
};
