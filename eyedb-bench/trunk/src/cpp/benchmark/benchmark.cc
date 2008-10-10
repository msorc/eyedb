#include <ctype.h>
#include <iostream>
#include <fstream>
#include "benchmark.h"

using namespace eyedb::benchmark;
using namespace std;

void Benchmark::loadProperties( const string &filename)
{
  ifstream infile;

  infile.exceptions( ifstream::badbit );
  try {
    infile.open (filename.c_str());
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

void Benchmark::printProperties()
{
  map<const string, string>::const_iterator begin = properties.begin();
  map<const string, string>::const_iterator end = properties.end();

  while (begin != end) {
    cout << (*begin).first << "=" << (*begin).second << endl;
    begin++;
  }
}

void Benchmark::bench()
{
  prepare();
  stopwatch.start();
  run();
  stopwatch.stop();
  finish();
}
