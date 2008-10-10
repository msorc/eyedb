#include <iostream>
#include <benchmark.h>

using namespace eyedb::benchmark;
using namespace std;

class TestBenchmark: public Benchmark {
public:
  virtual const char* getName() { return "Test"; }
  virtual const char* getDescription() { return "A test benchmark";};

  virtual void prepare() {}
  virtual void run() {}
  virtual void finish() {}
};

int main( int ac, char **av)
{
  TestBenchmark b;

  b.loadProperties( av[1]);
  b.printProperties();
}
