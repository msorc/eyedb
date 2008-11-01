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

  if (ac > 1)
    b.loadProperties( av[1]);
  else
    b.loadProperties( "test.properties");

  cout << "== Printing properties" << endl;
  b.printProperties();

  cout << "== Printing some values" << endl;
  int intValue;
  b.getIntProperty("two", intValue);
  cout << "two=" << intValue << endl;
  vector<int> count;
  b.getIntProperty("count", count);
  for (int i = 0; i < count.size(); i++)
    cout << "count[" << i << "]=" << count[i] << endl;

  string strValue;
  b.getStringProperty("url", strValue);
  cout << "url=" << strValue << endl;
  b.getStringProperty("database", strValue);
  cout << "database=" << strValue << endl;
}
