#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

  // Print results
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

void Benchmark::lexicalError( char c)
{
  cerr << "Error parsing property file: invalid character '" << c << "'\n";
}

void Benchmark::loadProperties( istream &is)
{
  char c;
  enum { BEGIN, NAME1, NAME, SKIP1, EQUAL, SKIP2, VALUE, SKIP3 } state;
  string name, value;

  state = BEGIN;
  while ( true) {
    c = is.get();

    if (is.eof())
      break;

    switch (state) {
    case BEGIN:
      if (isblank(c) || c == '\n') {
      } else if (isalpha(c)) {
	is.unget();
	state = NAME1;
      } else
	lexicalError( c);
      break;
    case NAME1:
      if (isalpha(c)) {
	name += c;
	state = NAME;
      } else
	lexicalError( c);
      break;
    case NAME:
      if (isalnum(c) || c == '.') {
	name += c;
	state = NAME;
      } else if (isblank(c))
	state = SKIP1;
      else if (c == '=') {
	is.unget();
	state = EQUAL;
      } else
 	lexicalError( c);
      break;
    case SKIP1:
      if (isblank(c)) {
      } else if (c == '=') {
	is.unget();
	state = EQUAL;
      } else
	lexicalError( c);
      break;
    case EQUAL:
      if (c == '=') {
	state = SKIP2;
      } else
	lexicalError( c);
      break;
    case SKIP2:
      if (isblank(c)) {
      } else if (isalnum(c)) {
	is.unget();
	state = VALUE;
      } else
	lexicalError( c);
      break;
    case VALUE:
      if (isspace(c)) {
	is.unget();
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
	lexicalError( c);
      break;
    }
  }
}

int Benchmark::getIntProperty( const std::string &name, int &value, int defaultValue)
{
  value = defaultValue;

  if (properties.find( name) == properties.end())
    return 0;

  std::istringstream iss(properties[name]);

  if ( iss >> value && iss.eof())
    return 1;

  return 0;
}

int Benchmark::getIntProperty( const std::string &name, std::vector<int> &values)
{
  values.clear();

  if (properties.find( name) == properties.end())
    return 0;

  istringstream iss(properties[name]);

  string s;
  while ( std::getline( iss, s, ',' ) ) {
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

