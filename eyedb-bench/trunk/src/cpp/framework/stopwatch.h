#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <vector>
#include <string>

namespace eyedb {
  namespace benchmark {
    class StopWatch {
    public:
      class Lap {
      public:
	Lap( const std::string &name, unsigned long time) 
	  : name( name), time( time) 
	{
	}

	unsigned long getTime() const
	{
	  return time;
	}

	const std::string& getName() const
	{
	  return name; 
	}

      private:
	std::string name;
	unsigned long time;
      };

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

      int getLapCount() const 
      {
	return laps.size(); 
      }

      const std::string& getLapName( int i) const
      {
	return laps[i].getName(); 
      }

      unsigned long getLapTime( int i) const
      {
	return laps[i].getTime(); 
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
