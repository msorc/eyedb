#include "stopwatch.h"
#include <sys/time.h>
#include <time.h>
#include <iostream>

using namespace eyedb::benchmark;

void StopWatch::start()
{
  running = true;

  startTime = currentTime( true);

  lapTime = 0;
}

unsigned long StopWatch::stop()
{
  if (running) {
    running = false;

    return currentTime();
  }

  return 0;
}

unsigned long StopWatch::lap( const std::string &name)
{
  unsigned long t = currentTime();

  unsigned long delta = t - lapTime;

  laps.push_back( Lap( name, delta));

  lapTime = t;

  return delta;
}

unsigned long StopWatch::currentTime( bool init)
{
  struct timeval now;

  gettimeofday( &now, NULL);

  unsigned long t = 1000 * now.tv_sec + now.tv_usec / 1000;

  if (!init)
    t -= startTime;

  return  t;
}
