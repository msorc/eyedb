#include <iostream>
#include <benchmark.h>

using namespace eyedb::benchmark;
using namespace std;

class TestBenchmark: public Benchmark {
public:
  virtual const char* getName() const { return "Test"; }
  virtual const char* getDescription() const { return "A test benchmark";};
  virtual const char* getRunDescription() const { return "C++ implementation";};

  virtual void prepare() {}
  virtual void run() {}
  virtual void finish() {}
};

int main( int ac, char **av)
{
  TestBenchmark b;

  if (ac > 1)
    b.getProperties().load( av[1]);
  else
    b.getProperties().load( "test.properties");

  cout << "== Printing properties" << endl;
  b.getProperties().print();

  cout << "== Printing some values" << endl;
  int intValue;
  b.getProperties().getIntProperty("test.two", intValue);
  cout << "test.two=" << intValue << endl;
  vector<int> count;
  b.getProperties().getIntProperty("count", count);
  for (int i = 0; i < count.size(); i++)
    cout << "count[" << i << "]=" << count[i] << endl;

  string strValue;
  b.getProperties().getStringProperty("url", strValue);
  cout << "url=" << strValue << endl;
  b.getProperties().getStringProperty("database", strValue);
  cout << "database=" << strValue << endl;
}
