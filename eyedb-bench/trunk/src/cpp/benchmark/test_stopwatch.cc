#include <stopwatch.h>
#include <iostream>
#include <unistd.h>

int active_wait( int n)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    s += i * i;
  
  return s;
}

int test( int n)
{
  eyedb::benchmark::StopWatch w;
  int r = 0;

  w.start();

  sleep( 1);
  w.lap("ONE");

  for (int i = 0; i < n; i++)
    r = active_wait(i);
  w.lap("TWO");

  for (int i = 0; i < 2*n; i++)
    r = active_wait(i);
  w.lap("THREE");

  w.stop();

  for ( int i = 0; i < w.getLapCount(); i++)
    std::cout << "lap[" << i << "] " << w.getLapName(i) << " " << w.getLapTime( i) << std::endl;

  std::cout << "Total " << w.getTotalTime() << std::endl;

  return r;
}

int main()
{
  test(20000);
}
