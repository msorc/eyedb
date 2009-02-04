#ifndef _EYEDB_BENCHMARK_PROPERTIES_
#define _EYEDB_BENCHMARK_PROPERTIES_

#include <iostream>
#include <map>
#include <vector>

namespace eyedb {
  namespace benchmark {
    class Properties {
    public:
      Properties() {}
      Properties( const Properties &x) : properties( x.properties) {}

      Properties &operator = (const Properties &x);

      void load( const std::string &filename);
      void load( std::istream &is);
      void load( int &argc, char **argv);

      int getBoolProperty( const std::string &name, bool &value, bool defaultValue = false) const;

      int getIntProperty( const std::string &name, int &value, int defaultValue = -1) const;
      int getIntProperty( const std::string &name, std::vector<int> &values) const;

      int getStringProperty( const std::string &name, std::string &value, const std::string &defaultValue = "") const;

      void print() const;

    private:
      void lexicalError( char c, int pos, int line);

      std::map<const std::string, std::string> properties;
    };
  };
};

#endif
