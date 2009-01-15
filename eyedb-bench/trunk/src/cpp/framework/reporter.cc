#include <iostream>
#include <sstream>
#include <iomanip>
#include "reporter.h"

using namespace eyedb::benchmark;
using namespace std;

const int SimpleReporter::defaultColumnWidth = 10;
const char SimpleReporter::defaultColumnSeparator = ',';

SimpleReporter::SimpleReporter() 
  : columnWidth( defaultColumnWidth), columnSeparator( defaultColumnSeparator)
{
}

void SimpleReporter::report( const Benchmark &benchmark)
{
  cout << endl;
  cout << "Bench: " << benchmark.getName() << endl;
  cout << benchmark.getDescription() << endl;
  cout << endl;
  cout << benchmark.getImplementation() << endl;
  cout << endl;

  reportResult( benchmark.getResult());

  cout << endl;
}

void SimpleReporter::reportResult( const Result &result)
{
  vector<string>::const_iterator it;
  int i;

  for ( it = result.getHeaders().begin(), i = 0; it < result.getHeaders().end(); it++ ) {
    cout << setiosflags(ios::right) << setw( columnWidth) << *it;
    if (i != result.getHeaders().size() - 1)
      cout << columnSeparator;
  }

  cout << endl;

  for (int i = 0; i < result.size(); i++) {

    vector<unsigned long>::const_iterator it;
    int j;

    for ( it = result.getValues(i).begin(), j = 0; it < result.getValues(i).end(); it++ ) {
      cout << setiosflags(ios::right) << setw( columnWidth) << *it;
      if (i != result.getValues(i).size() - 1)
	cout << columnSeparator;
    }

    cout << endl;
  }
}

