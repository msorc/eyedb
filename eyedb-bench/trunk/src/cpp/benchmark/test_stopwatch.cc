#include <stopwatch.h>
#include <iostream>
#include <unistd.h>

int main()
{
  eyedb::benchmark::StopWatch w;

  w.start();
  for ( int i = 0; i < 3; i++) {
    sleep( 1);
    w.lap("LAP");
  }
  std::cout << "Total " << w.stop() << std::endl;

  for ( int i = 0; i < w.getLapCount(); i++)
    std::cout << "lap[" << i << "] " << w.getLapName(i) << " " << w.getLapTime( i) << std::endl;
}
