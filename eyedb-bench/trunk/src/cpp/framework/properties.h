#ifndef _EYEDB_BENCHMARK_PROPERTIES_
#define _EYEDB_BENCHMARK_PROPERTIES_

#include <iostream>
#include <map>
#include <vector>

namespace eyedb {
  namespace benchmark {
    class Properties {
    public:

      void load( const std::string &filename);
      void load( std::istream &is);
      void load( int &argc, char **argv);

      int getIntProperty( const std::string &name, int &value, int defaultValue = -1);
      int getIntProperty( const std::string &name, std::vector<int> &values);

      int getStringProperty( const std::string &name, std::string &value, const std::string &defaultValue = "");

      void print();

    private:
      void lexicalError( char c, int pos, int line);

      std::map<const std::string, std::string> properties;
    };
  };
};

#endif
