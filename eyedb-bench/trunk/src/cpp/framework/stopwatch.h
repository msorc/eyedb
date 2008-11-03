#ifndef _EYEDB_BENCHMARK_STOPWATCH_H_
#define _EYEDB_BENCHMARK_STOPWATCH_H_

#include <vector>
#include <string>

namespace eyedb {
  namespace benchmark {

    typedef std::pair<std::string,unsigned long> Lap;

    class StopWatch {
    public:
      StopWatch() 
	: running( false), startTime(0), totalTime(0), lapTime(0) 
      {
      }

      void start();
      unsigned long stop();
      unsigned long lap( const std::string &name);
      void reset();

      unsigned long getTotalTime() const
      {
	return totalTime; 
      }

      const std::vector<Lap> &getLaps()
      {
	return laps;
      }

      int getLapCount() const 
      {
	return laps.size(); 
      }

      const std::string& getLapName( int i) const
      {
	return laps[i].first; 
      }

      unsigned long getLapTime( int i) const
      {
	return laps[i].second; 
      }

    private:
      unsigned long systemTime();
      unsigned long time()
      { 
	return systemTime() - startTime; 
      }

      bool running;
      unsigned long startTime;
      unsigned long totalTime;
      unsigned long lapTime;
      std::vector<Lap> laps;
    };
  };
};

#endif
