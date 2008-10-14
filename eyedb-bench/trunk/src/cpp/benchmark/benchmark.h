#include <iostream>
#include <map>
#include "stopwatch.h"

namespace eyedb {
  namespace benchmark {
    class Benchmark {
    public:

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

      void loadProperties( const std::string &filename);
      void loadProperties( std::istream &is);

      int getIntProperty( const std::string &name, int &value, int defaultValue = -1);
      int getIntProperty( const std::string &name, std::vector<int> &values);

      int getStringProperty( const std::string &name, std::string &value, const std::string &defaultValue = "");

      void printProperties();

    private:
      void lexicalError( char c);

      StopWatch stopwatch;
      std::map<const std::string, std::string> properties;
    };
  };
};
