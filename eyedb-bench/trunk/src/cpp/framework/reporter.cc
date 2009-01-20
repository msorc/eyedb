#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "reporter.h"

using namespace eyedb::benchmark;
using namespace std;

DefaultReporter::DefaultReporter()
{
  reporters.push_back( new SimpleReporter() );
  reporters.push_back( new CSVReporter() );
}

void DefaultReporter::report(const Benchmark &benchmark)
{
  vector<Reporter *>::iterator it;

  for ( it = reporters.begin(); it < reporters.end(); it++ ) {
    (*it)->report( benchmark);
  }
}

const int SimpleReporter::defaultColumnWidth = 10;
const char SimpleReporter::defaultColumnSeparator = ',';

void SimpleReporter::report( const Benchmark &benchmark)
{
  int w;
  if (benchmark.getProperties().getIntProperty("reporter.simple.column_width", w))
    setColumnWidth( w);
  string c;
  if (benchmark.getProperties().getStringProperty("reporter.simple.column_separator", c))
    setColumnSeparator( c[0]);

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

const char CSVReporter::defaultColumnSeparator = ',';

void CSVReporter::report( const Benchmark &benchmark)
{
  string c;
  if (benchmark.getProperties().getStringProperty("reporter.csv.column_separator", c))
    setColumnSeparator( c[0]);

  string filename;
  if (!benchmark.getProperties().getStringProperty("reporter.csv.filename", filename))
    filename = "/var/tmp/eyedb-benchmark.csv";

  bool append;
  benchmark.getProperties().getBoolProperty("reporter.csv.append", append);

  ios_base::openmode mode = ios_base::out;
  if (append)
    mode |= (ios_base::ate | ios_base::app);

  outfile.open(filename.c_str(), mode);

  long pos = outfile.tellp();

  if (pos == 0) {
    outfile << "\"Bench:\"" << columnSeparator << "\"" << benchmark.getName() << "\"" << endl;
    outfile << "\"" << benchmark.getDescription() << "\"" << endl;
    outfile << endl;
  }

  outfile << "\"" << benchmark.getImplementation() << "\"" << endl;

  reportResult( benchmark.getResult());

  outfile << endl;

  outfile.close();
}

void CSVReporter::reportResult( const Result &result)
{
  vector<string>::const_iterator it;
  int i;

  for ( it = result.getHeaders().begin(), i = 0; it < result.getHeaders().end(); it++ ) {
    outfile << "\"" << *it << "\"";
    if (i != result.getHeaders().size() - 1)
      outfile << columnSeparator;
  }

  outfile << endl;

  for (int i = 0; i < result.size(); i++) {

    vector<unsigned long>::const_iterator it;
    int j;

    for ( it = result.getValues(i).begin(), j = 0; it < result.getValues(i).end(); it++ ) {
      outfile << "\"" << *it << "\"";
      if (i != result.getValues(i).size() - 1)
	outfile << columnSeparator;
    }

    outfile << endl;
  }
}

