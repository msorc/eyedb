#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "benchmark.h"

using namespace eyedb::benchmark;
using namespace std;

void Benchmark::bench()
{
  cout << "----------------------------------------------------------------------" << endl;
  cout << getName() << endl;
  cout << getDescription() << endl;
  cout << "----------------------------------------------------------------------" << endl;

  prepare();
  stopwatch.start();
  run();
  stopwatch.stop();
  finish();

  cout << endl;
}

void Benchmark::report()
{
  for ( int i = 0; i < getStopwatch().getLapCount(); i++)
    cout << "[" << setiosflags(ios::left) << setw(10) << getStopwatch().getLapName(i) << "] " << getStopwatch().getLapTime( i) << "ms" << endl;

  cout << "[" << setiosflags(ios::left) << setw(10) << "total" << "] " << getStopwatch().getTotalTime() << "ms" << endl;
  cout << endl;
}

void Benchmark::loadProperties( const string &filename)
{
  ifstream infile;

  infile.exceptions( ifstream::badbit );
  try {
    infile.open (filename.c_str());
    if (infile.fail()) {
      cerr << "Error reading properties file " << filename << endl;
      return;
    }

    loadProperties( infile);
  }
  catch (ifstream::failure e) {
    cerr << "Error reading properties file " << filename << endl;
  }

  infile.close();
}

void Benchmark::lexicalError( char c, int pos, int line)
{
  cerr << "Error parsing property file: invalid character '" << c << "' near position " << pos << " on line " << line << endl;
}

static char getchar( istream &is, int &pos, int &line)
{
  char c = is.get();

  if (c == '\n') {
    line++;
    pos = 0;
  }
  else
    pos++;

  return c;
}

static void ungetchar( istream &is, char c, int &pos, int &line)
{
  is.unget();

  if (c == '\n') {
    line--;
    pos = 0;
  }
  else
    pos--;
}

void Benchmark::loadProperties( istream &is)
{
  char c;
  enum { BEGIN, NAME1, NAME, SKIP1, EQUAL, SKIP2, VALUE, SKIP3 } state;
  string name, value;
  int pos, line;

  state = BEGIN;
  line = pos = 1;
  while ( true) {
    c = getchar(is, pos, line);

    if (is.eof())
      break;

    switch (state) {
    case BEGIN:
      if (isblank(c) || c == '\n') {
      } else if (isalpha(c)) {
	ungetchar( is, c, pos, line);
	state = NAME1;
      } else
	lexicalError( c, pos, line);
      break;
    case NAME1:
      if (isalpha(c)) {
	name += c;
	state = NAME;
      } else
	lexicalError( c, pos, line);
      break;
    case NAME:
      if (isalnum(c) || c == '.' || c == '_') {
	name += c;
	state = NAME;
      } else if (isblank(c))
	state = SKIP1;
      else if (c == '=') {
	ungetchar( is, c, pos, line);
	state = EQUAL;
      } else
 	lexicalError( c, pos, line);
      break;
    case SKIP1:
      if (isblank(c)) {
      } else if (c == '=') {
	ungetchar( is, c, pos, line);
	state = EQUAL;
      } else
	lexicalError( c, pos, line);
      break;
    case EQUAL:
      if (c == '=') {
	state = SKIP2;
      } else
	lexicalError( c, pos, line);
      break;
    case SKIP2:
      if (isblank(c)) {
      } else if (isalnum(c)) {
	ungetchar( is, c, pos, line);
	state = VALUE;
      } else
	lexicalError( c, pos, line);
      break;
    case VALUE:
      if (isspace(c)) {
	ungetchar( is, c, pos, line);
	properties[name] = value;
	name.clear();
	value.clear();
	state = SKIP3;
      } else {
	value += c;
      }
      break;
    case SKIP3:
      if (isblank(c)) {
      } else if (c == '\n') {
	state = BEGIN;
      } else
	lexicalError( c, pos, line);
      break;
    }
  }
}

int Benchmark::getIntProperty( const string &name, int &value, int defaultValue)
{
  value = defaultValue;

  if (properties.find( name) == properties.end())
    return 0;

  istringstream iss(properties[name]);

  if ( iss >> value && iss.eof())
    return 1;

  return 0;
}

int Benchmark::getIntProperty( const string &name, vector<int> &values)
{
  values.clear();

  if (properties.find( name) == properties.end())
    return 0;

  istringstream iss(properties[name]);

  string s;
  while ( getline( iss, s, ',' ) ) {
    istringstream iss2(s);
    int value;

    if ( iss2 >> value && iss2.eof())
      values.push_back( value);
    else
      return 0;
  }
  
  return 1;
}

int Benchmark::getStringProperty( const string &name, string &value, const string &defaultValue)
{
  if (properties.find( name) == properties.end()) {
    value.assign( defaultValue);
    return 0;
  }

  value.assign( properties[name]);

  return 1;
}

void Benchmark::printProperties()
{
  map<const string, string>::const_iterator begin = properties.begin();
  map<const string, string>::const_iterator end = properties.end();

  while (begin != end) {
    cout << (*begin).first << "=" << (*begin).second << endl;
    begin++;
  }
}

