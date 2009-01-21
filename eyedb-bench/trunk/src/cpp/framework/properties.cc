#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "properties.h"

using namespace eyedb::benchmark;
using namespace std;

Properties &Properties::operator=( const Properties &x)
{
  if (&x == this)
    return *this;

  properties = x.properties;

  return *this;
}

void Properties::load( const string &filename)
{
  ifstream infile;

  infile.exceptions( ifstream::badbit );
  try {
    infile.open (filename.c_str());
    if (infile.fail()) {
      cerr << "Error reading properties file " << filename << endl;
      return;
    }

    load( infile);
  }
  catch (ifstream::failure e) {
    cerr << "Error reading properties file " << filename << endl;
  }

  infile.close();
}

void Properties::lexicalError( char c, int pos, int line)
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

void Properties::load( istream &is)
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
      } else {
	ungetchar( is, c, pos, line);
	state = VALUE;
      } 
      break;
    case VALUE:
      if (c == '\n') {
	properties[name] = value;
	name.clear();
	value.clear();
	state = BEGIN;
      } else {
	value += c;
      }
      break;
    }
  }
}

void Properties::load( int &argc, char **argv)
{
  for (int i = 0; i < argc; ) {
    if (!strncmp( argv[i], "-D", 2)) {
	char *s = argv[i] + 2;
	char *v = strchr( s, '=');

	if (v != NULL) {
	  *v = '\0';
	  properties[s] = v+1;

	  for ( int j = i; j < argc-1; j++)
	    argv[j] = argv[j+1];
	  argc--;
	}
      }
    else
      i++;
  }
}

int Properties::getBoolProperty( const std::string &name, bool &value, bool defaultValue) const
{
  value = defaultValue;

  map<const string, string>::const_iterator it = properties.find( name);

  if (it == properties.end())
    return 0;

  istringstream iss(it->second);

  return it->second == "true" || it->second == "1";

  return 0;
}

int Properties::getIntProperty( const string &name, int &value, int defaultValue) const
{
  value = defaultValue;

  map<const string, string>::const_iterator it = properties.find( name);

  if (it == properties.end())
    return 0;

  istringstream iss(it->second);

  if ( iss >> value && iss.eof())
    return 1;

  return 0;
}

int Properties::getIntProperty( const string &name, vector<int> &values) const
{
  values.clear();

  map<const string, string>::const_iterator it = properties.find( name);

  if (it == properties.end())
    return 0;

  istringstream iss(it->second);

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

int Properties::getStringProperty( const string &name, string &value, const string &defaultValue) const
{
  map<const string, string>::const_iterator it = properties.find( name);

  if (it == properties.end()) {
    value.assign( defaultValue);
    return 0;
  }

  value.assign( it->second);

  return 1;
}

void Properties::print() const
{
  map<const string, string>::const_iterator begin = properties.begin();
  map<const string, string>::const_iterator end = properties.end();

  while (begin != end) {
    cout << (*begin).first << "=" << (*begin).second << endl;
    begin++;
  }
}

