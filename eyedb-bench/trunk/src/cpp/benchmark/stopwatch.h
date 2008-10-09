#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include <vector>
#include <string>

namespace eyedb {
  namespace benchmark {
    class StopWatch {
    public:
      class Lap {
	friend class StopWatch;
      public:
	Lap( const std::string &name, unsigned long time) : name( name), time( time) {}
	unsigned long getTime() { return time; }
	const std::string& getName() { return name; }
      private:
	std::string name;
	unsigned long time;
      };

      StopWatch() : running( false) {}

      void start();
      unsigned long stop();
      unsigned long lap( const std::string &name);
      void reset() { running = false; startTime = 0; }

      int getLapCount() { return laps.size(); }
      const std::string& getLapName( int i) { return laps[i].name; }
      unsigned long getLapTime( int i) { return laps[i].time; }

    private:
      unsigned long startTime;
      unsigned long currentTime( bool init = false);

      bool running;

      unsigned long lapTime;

      std::vector<Lap> laps;
    };
  };
};

#endif
