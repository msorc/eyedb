#include "benchmark.h"

using namespace eyedb::benchmark;
using namespace std;

void Benchmark::bench()
{
//   reportBegin();
  prepare();
  stopwatch.start();
  run();
  stopwatch.stop();
  finish();
//   reportEnd();
}

