#include "benchmark.h"

using namespace eyedb::benchmark;
using namespace std;

void Benchmark::bench()
{
  prepare();
  run();
  finish();
}

