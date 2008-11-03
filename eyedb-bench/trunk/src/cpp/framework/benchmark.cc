#include "benchmark.h"

using namespace eyedb::benchmark;
using namespace std;

void Benchmark::bench()
{
  prepare();
  stopwatch.start();
  run();
  stopwatch.stop();
  finish();
}

