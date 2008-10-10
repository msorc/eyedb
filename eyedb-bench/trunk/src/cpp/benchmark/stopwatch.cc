#include "stopwatch.h"
#include <sys/time.h>
#include <time.h>
#include <iostream>

using namespace eyedb::benchmark;

void StopWatch::start()
{
  running = true;
  startTime = systemTime();
  lapTime = 0;
}

unsigned long StopWatch::stop()
{
  if (running) {
    running = false;
    totalTime = time();
  } else
    totalTime = 0;

  return totalTime;
}

unsigned long StopWatch::lap( const std::string &name)
{
  unsigned long t = time();
  unsigned long delta = t - lapTime;
  laps.push_back( Lap( name, delta));
  lapTime = t;

  return delta;
}

void StopWatch::reset()
{
  running = false; 
  startTime = totalTime = lapTime = 0;
  laps.clear();
}

unsigned long StopWatch::systemTime()
{
  struct timeval now;

  gettimeofday( &now, NULL);

  return 1000 * now.tv_sec + now.tv_usec / 1000;
}
