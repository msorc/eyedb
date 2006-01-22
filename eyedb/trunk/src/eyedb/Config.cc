/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eyedb/eyedb.h>
#include "eyedblib/strutils.h"
#include "lib/compile_builtin.h"

#define MAXFILES 16

namespace eyedb {

  Config *Config::theClientConfig = 0;
  Config *Config::theServerConfig = 0;

  static Bool initialized = False;

  /*
   * Config file parser
   */

  static int fd_w;
  static FILE *fd;
  static int *pline;
  static const char *pfile;

  static FILE *fd_sp[MAXFILES];
  static int line[MAXFILES];
  static char *file_sp[MAXFILES];

  static const char *
  uppercase(const char *s)
  {
    static char buf[128];
    char c, *p;
    p = buf;

    while (c = *s++)
      *p++ = c + (c >= 'a' && c <= 'z' ? ('A' - 'a') : 0);

    *p = 0;
    return buf;
  }

  static int
  skip_spaces()
  {
    for (;;)
      {
	char c = fgetc(fd);
	if (c == EOF)
	  return 0;

	if (c != ' ' && c != '\t' && c != '\n')
	  {
	    ungetc(c, fd);
	    break;
	  }

	if (c == '\n')
	  (*pline)++;
      }

    return 1;
  }
  
  static int
  skip_comments()
  {
    char c = fgetc(fd);
    if (c == '#')
      for (;;)
	{
	  c = fgetc(fd);
	  if (c == EOF)
	    return 0;
	
	  if (c == '\n')
	    {
	      ungetc(c, fd);
	      break;
	    }
	}
    else
      ungetc(c, fd);

    return 1;
  }

  static const char assign[] = "=";
  static const char term[] = ";";

  static int
  check_spe(int &is_spe)
  {
    char c;

    is_spe = 0;
    c = fgetc(fd);
    if (c == EOF)
      return 0;

    if (c == ';')
      {
	is_spe = 1;
	return 1;
      }
    else if (c == '=')
      {
	is_spe = 2;
	return 1;
      }

    ungetc(c, fd);
    return 1;
  }

  static inline const char *
  line_str()
  {
    static std::string str;
    if (!pline || !pfile)
      return "";

    str = std::string("file \"" ) + pfile + "\" near line " + str_convert((long)*pline) + ": ";
    return str.c_str();
  }

  static void
  error(const char *msg)
  {
    std::string s = std::string("file \"") + file_sp[fd_w-1] + "\", " + line_str() +
      "syntax error: " + msg;

    if (initialized)
      {
	Exception::Mode mode = Exception::setMode(Exception::ExceptionMode);
	(void)Exception::make(IDB_ERROR, s.c_str());
	Exception::setMode(mode);
	return;
      }

    fprintf(stderr, "%s\n", s.c_str());
    exit(1);
  }

  static void
  error(const std::string &s)
  {
    error(s.c_str());
  }

  static void
  error(const char *fmt, const char *x1, const char *x2 = 0, const char *x3 = 0)
  {
    if (initialized)
      {
	Exception::Mode mode = Exception::setMode(Exception::ExceptionMode);
	(void)Exception::make(IDB_ERROR, fmt, x1, x2, x3);
	Exception::setMode(mode);
	return;
      }

    fprintf(stderr, fmt, x1, x2, x3);
    fprintf(stderr, "\n");
    exit(1);
  }

  static int
  push_file(const char *file, int quietFileNotFoundError)
  {
    if (strlen(file) > 2 && file[0] == '/' && file[1] == '/')
      {
	file += 2;
	std::string s =  std::string( eyedblib::CompileBuiltin::getSysconfdir()) + "/" + file;
	fd = fopen( s.c_str(), "r");
      }
    else
      fd = fopen(file, "r");

    if (!fd)
      {
	if (quietFileNotFoundError)
	  return 0;
	else
	  error("%scannot open file '%s' for reading",
	        line_str(), file);
      }

    pline = &line[fd_w];
    fd_sp[fd_w] = fd;
    file_sp[fd_w] = strdup(file);
    pfile = file;
    *pline = 1;
    fd_w++;

    return 1;
  }

  static const char *
  nexttoken_realize(Config *config)
  {
    static char tok[512];
    char c;

    if (!skip_spaces())
      return 0;

    int is_spe;

    if (!check_spe(is_spe))
      return 0;
    if (is_spe == 1)
      return term;
    if (is_spe == 2)
      return assign;

    char *p = tok;
    int force = 0;
    int hasvar = 0;
    int backslash = 0;
    char svar[128];
    char *var = 0;

    for (;;)
      {
	if (!force && !skip_comments())
	  return 0;

	c = fgetc(fd);
	if (c == EOF)
	  return 0;

	if (c == '%')
	  {
	    if (!var)
	      {
		var = svar;
		continue;
	      }

	    *var = 0;
	    *p = 0;

	    if (!*svar)
	      strcat(tok, "%");
	    else
	      {
		const char *val = config->getValue(svar);
		if (!val)
		  error("%sunknown configuration variable '%s'", line_str(), svar);
		strcat(tok, val);
	      }

	    p = tok + strlen(tok);
	    var = 0;
	    hasvar = 1;
	    continue;
	  }

	if (var)
	  {
	    if (var - svar >= sizeof(svar))
	      {
		svar[sizeof(svar)-1] = 0;
		error("%sconfiguration variable too long: '%s' "
		      "(maximum size is %s)", line_str(), svar,
		      str_convert((long)sizeof(svar)-1).c_str());
	      }

	    *var++ = c;
	    continue;
	  }

	if (c == '"' && !force)
	  {
	    force = 1;
	    continue;
	  }

	if (force && c == '\\')
	  {
	    backslash = 1;
	    continue;
	  }

	if (c == '\n')
	  {
	    (*pline)++;
	    if (!force)
	      break;
	  }
	else if (!force)
	  {
	    if (c == ' ' || c == '\t')
	      break;
	    else if (c == ';' || c == '=')
	      {
		ungetc(c, fd);
		break;
	      }
	  }
	else
	  {
	    if (backslash)
	      {
		if (c == 'n')
		  c = '\n';
		else if (c == 'a')
		  c = '\a';
		else if (c == 'b')
		  c = '\b';
		else if (c == 'f')
		  c = '\f';
		else if (c == 'r')
		  c = '\r';
		else if (c == 't')
		  c = '\t';
		else if (c == 'v')
		  c = '\v';
		else if (c == '\\')
		  c = '\\';

		backslash = 0;
	      }
	    else if (c == '"')
	      break;
	  }

	*p++ = c;
      }

    *p = 0;

    return (p != tok || force || hasvar) ? tok : nexttoken_realize(config);
  }

  static const char *
  nexttoken(Config *config)
  {
    const char *p = nexttoken_realize(config);

    if (!p)
      {
	if (fd_w > 0 && --fd_w > 0)
	  {
	    fd = fd_sp[fd_w-1];
	    pline = &line[fd_w-1];
	    pfile = file_sp[fd_w-1];
	    return nexttoken(config);
	  }

	return 0;
      }

    if (!strcmp(p, "include"))
      {
	const char *file = nexttoken_realize(config);
	if (!file)
	  {
	    error("file name expected after include");
	    return 0;
	  }

	push_file(file, 0);
	return nexttoken(config);
      }

    return p;
  }

  void Config::add(const char *file, int quietFileNotFoundError)
  {
    if (!push_file(file, quietFileNotFoundError))
      return;

    int state = 0;
    char *name = 0, *value = 0;

    for (;;)
      {
	const char *p = nexttoken(this);

	if (!p)
	  return;

	switch(state)
	  {
	  case 0:
	    if (!strcmp(p, assign) || !strcmp(p, term))
	      error(std::string("unexpected '") + p + "'");
	    name = strdup(p);
	    state = 1;
	    break;

	  case 1:
	    if (strcmp(p, assign))
	      error(std::string("'") + assign + "' expected, got '" + p + "'");
	    state = 2;
	    break;

	  case 2:
	    if (!strcmp(p, assign) || !strcmp(p, term))
	      error(std::string("unexpected '") + p + "'");
	    value = strdup(p);
	    state = 3;
	    break;

	  case 3:
	    if (strcmp(p, term))
	      error(std::string("'") + term + "' expected, got '" + p + "'");
	    setValue( name, value);
	    free(name);
	    free(value);
	    name = 0;
	    value = 0;
	    state = 0;
	    break;
	  }
      }
  }


  /*
   * Config init static method
   * Is there mainly so that error() functions work... but probably it is no longer needed
   */
  void
  Config::init()
  {
    if (initialized) 
      return;

    initialized = True;
  }

  /*
   * Config and Config::Item constructors, destructor and operator=
   */

  Config::Config()
    : list()
  {
  }

  // @@@ FIXME: this constructor does not set the defaults
  Config::Config(const char *file)
  {
    add(file);
  }

  Config::Config(const Config &config)
  {
    *this = config;
  }

  void
  Config::garbage()
  {
    LinkedListCursor c(list);
    Item *item;

    while (c.getNext((void *&)item))
      delete item;

    list.empty();
  }

  Config& Config::operator=(const Config &config)
  {
    garbage();

    LinkedListCursor c(config.list);
    Item *item;
    while(c.getNext((void *&)item))
      list.insertObjectFirst(new Item(*item));

    return *this;
  }

  Config::~Config()
  {
    garbage();
  }

  Config::Item& Config::Item::operator=(const Item &item)
  {
    if (this == &item)
      return *this;

    free(name);
    free(value);
    name = strdup(item.name);
    value = strdup(item.value);
    return *this;
  }

  Config::Item::Item()
  {
    name = 0;
    value = 0;
  }

  Config::Item::Item(const char *_name, const char *_value)
  {
    name = strdup(_name);
    value = strdup(_value);
  }

  Config::Item::Item(const Item &item)
  {
    name = strdup(item.name);
    value = strdup(item.value);
  }

  Config::Item::~Item()
  {
    free(name);
    free(value);
  }


  /*
   * Config operator<<
   */

  std::ostream& operator<<( std::ostream& os, const Config& config)
  {
    LinkedListCursor c(config.list);
    Config::Item *item;

    while (c.getNext((void *&)item))
      {
	os << "name= " << item->name << " value= " << item->value << std::endl;
      }

    return os;
  }


  /*
   * Config static _release method
   */

  void
  Config::_release()
  {
    if (theClientConfig)
      delete theClientConfig;

    if (theServerConfig)
      delete theServerConfig;

    theClientConfig = 0;
    theServerConfig = 0;
  }


  /*
   * Config variable set and get
   */

  void 
  Config::setValue(const char *name, const char *value)
  {
    Item *item = new Item(name, value);
    list.insertObjectFirst(item);
  }

  const char *
  Config::getValue(const char *name)
  {
    const char *s = getenv((std::string("EYEDB") + uppercase(name)).c_str());
    if (s)
      return s;

    LinkedListCursor c(list);
    Item *item;

    while (c.getNext((void *&)item))
      if (!strcasecmp(item->name, name)) {
	return item->value;
      }

    return (const char *)0;
  }

  Config::Item *
  Config::getValues(int &item_cnt) const
  {
    item_cnt = list.getCount();

    if (!item_cnt)
      return (Item *)0;

    Item *items = new Item[list.getCount()];

    Item *item;
    LinkedListCursor c(list);

    int n;
    for (n = 0; c.getNext((void *&)item); )
      {
	int _not = 0;
	for (int i = 0; i < n; i++)
	  if (!strcmp(items[i].name, item->name))
	    {
	      _not = 1;
	      break;
	    }

	if (!_not)
	  items[n++] = *item;
      }

    item_cnt = n;
    return items;
  }


  /*
   * Client and server config management
   */

  static std::string
  getConfigFile( const char* environmentVariable, const char* configFilename)
  {
    const char* realname;

    realname = getenv( environmentVariable);
    if (realname)
      return realname;

    return std::string(eyedblib::CompileBuiltin::getSysconfdir()) + "/eyedb/" + configFilename;
  }

  static const std::string tcp_port = "6240";

  void
  Config::setClientDefaults()
  {
    std::string localstatedir = eyedblib::CompileBuiltin::getLocalstatedir();

    // Port
    setValue( "port", (localstatedir + "/lib/eyedb/pipes/eyedbd").c_str());

    // TCP Port
    setValue( "tcp_port", tcp_port.c_str());

    // Hostname
    setValue( "host", "localhost");

    // User
    setValue( "user", "@");

    // EYEDBDBM Database
    //setValue( "dbm", (localstatedir + "/lib/eyedb/db/dbmdb.dbs").c_str());
    setValue( "dbm", "default");

    // Bases directory
    setValue( "data_dir", (localstatedir + "/lib/eyedb/db").c_str());
  }

  void
  Config::setServerDefaults()
  {
    std::string libdir = eyedblib::CompileBuiltin::getLibdir();
    std::string localstatedir = eyedblib::CompileBuiltin::getLocalstatedir();
    std::string sysconfdir = eyedblib::CompileBuiltin::getSysconfdir();

    // Executables directory
    setValue( "bindir", eyedblib::CompileBuiltin::getBindir());

    // pipes:
    setValue( "pipedir", (localstatedir + "/lib/eyedb/pipes").c_str());

    // tmpdir
    setValue( "tmpdir", (localstatedir + "/lib/eyedb/tmp").c_str());

    // sopath
    setValue( "sopath", (libdir + "/eyedb").c_str());

    // Default EYEDBDBM Databases
    setValue( "default_dbm", (localstatedir + "/lib/eyedb/db/dbmdb.dbs").c_str());

    // Granted EYEDBDBM Databases
    //setValue( "granted_dbm", (localstatedir + "/lib/eyedb/db/dbmdb.dbs").c_str());
    // EV : 22/01/06
    // when variable expansion will be done in getValue(), granted_dbm will be:
    //setValue( "granted_dbm", "%default_dbm%");

    // Server Parameters
    setValue( "access_file", (sysconfdir + "/eyedb/Access").c_str());
    setValue( "smdport", (localstatedir + "/lib/eyedb/pipes/eyedbsmd").c_str());

    // Server Parameters
    setValue( "listen", ("localhost:" + tcp_port + "," + localstatedir + "/lib/eyedb/pipes/eyedbd").c_str());

    // OQL path
    setValue( "oqlpath", (libdir + "/eyedb/oql").c_str());
  }

  Config* 
  Config::getClientConfig()
  {
    if (theClientConfig)
      return theClientConfig;

    theClientConfig = new Config();

    theClientConfig->setClientDefaults();

    std::string configFile = getConfigFile( "EYEDBDCONF", "eyedbd.conf");

    theClientConfig->add( configFile.c_str(), 1);

    return theClientConfig;
  }
  
  Config* 
  Config::getServerConfig()
  {
    if (theServerConfig)
      return theServerConfig;

    theServerConfig = new Config();
    
    theServerConfig->setServerDefaults();

    std::string configFile = getConfigFile( "EYEDBCONF", "eyedb.conf");

    theServerConfig->add( configFile.c_str(), 1);

    return theServerConfig;
  }


  /*
   * Config public static methods
   */

  const char *
  Config::getServerValue(const char *name)
  {
    return Config::getServerConfig()->getValue(name);
  }

  const char *
  Config::getClientValue(const char *name)
  {
    return Config::getClientConfig()->getValue(name);
  }
}
