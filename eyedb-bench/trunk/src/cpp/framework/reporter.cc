#include <iostream>
#include <sstream>
#include <iomanip>
#include "reporter.h"

using namespace eyedb::benchmark;
using namespace std;

const int Reporter::defaultColumnWidth = 10;
const char Reporter::defaultColumnSeparator = ',';

Reporter::Reporter() 
  : columnWidth( defaultColumnWidth), columnSeparator( defaultColumnSeparator), 
    reportLapsDone(false), reportColumnHeadersDone(false) 
{
}

void Reporter::reportBegin( const Benchmark &benchmark)
{
  cout << endl;
  cout << "Bench: " << benchmark.getName() << endl;
  cout << benchmark.getDescription() << endl;
  cout << endl;
  cout << benchmark.getRunDescription() << endl;
}

void Reporter::reportLaps( const Benchmark &benchmark)
{
  reportLapsDone = true;

  if (!reportColumnHeadersDone) {
    reportColumnHeadersDone = true;

    if (columnHeaders.size() != 0) {
      for ( int i = 0; i < columnHeaders.size(); i ++) {
	cout << setiosflags(ios::right) << setw( columnWidth) << columnHeaders[i];
	if (i != columnHeaders.size() - 1)
	  cout << columnSeparator;
      }

      cout << endl;
    }
  }

  if (rowHeaders.size() != 0) {
    for ( int i = 0; i < rowHeaders.size(); i ++) {
      cout << setiosflags(ios::right) << setw( columnWidth) << rowHeaders[i] << columnSeparator;
    }
  }

  for ( int i = 0; i < benchmark.getStopwatch().getLapCount(); i++) {
    cout << setiosflags(ios::right) << setw( columnWidth) << benchmark.getStopwatch().getLapTime( i) << columnSeparator;
  }

  cout << setiosflags(ios::right) << setw( columnWidth) << benchmark.getStopwatch().getTotalTime() << endl;

  rowHeaders.clear();
}

void Reporter::reportEnd( const Benchmark &benchmark)
{
  if (!reportLapsDone)
    reportLaps( benchmark);

  cout << endl;
}

