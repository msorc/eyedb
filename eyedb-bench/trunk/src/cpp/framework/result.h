#ifndef _EYEDB_BENCHMARK_RESULT_
#define _EYEDB_BENCHMARK_RESULT_

#include <string>
#include <vector>
#include <utility>
#include "stopwatch.h"

namespace eyedb {
  namespace benchmark {
    class Result {
    public:

      Result()
	: lapHeadersAdded(false), current(0)
      {
	std::vector<unsigned long> v;

	rows.push_back(v);
      }

      void addHeader( const std::string &header)
      {
	headers.push_back( header);
      }

      void addValue( unsigned long value)
      {
	rows[current].push_back( value);
      }

      void addLaps( const std::vector<Lap> &laps)
      {
	if (!lapHeadersAdded) {
	  lapHeadersAdded = true;

	  std::vector<Lap>::const_iterator it;

	  for ( it= laps.begin() ; it < laps.end(); it++)
	    addHeader( it->first);
	}

	std::vector<Lap>::const_iterator it;

	for ( it= laps.begin() ; it < laps.end(); it++)
	  addValue( it->second);
      }

      void next()
      {
	current++;
	std::vector<unsigned long> v;

	rows.push_back(v);
      }

      const std::vector<std::string>& getHeaders() const
      {
	return headers;
      }

      const std::vector<unsigned long>& getValues( int i) const
      {
	return rows[i];
      }

      int size() const
      {
	return current;
      }

    private:
      std::vector<std::string> headers;

      typedef std::vector<unsigned long> vector_of_long;
      std::vector<vector_of_long> rows;
      int current;

      bool lapHeadersAdded;
    };
  };
};

#endif
