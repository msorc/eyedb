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

extern struct config_default {
  const char *name, *value;
} config_defaults[];

#define MAXFILES 16

namespace eyedb {

  static int init_done;
  static Config *the_config;

  static int fd_w;
  static FILE *fd;
  static int *pline;
  static const char *pfile;

  static FILE *fd_sp[MAXFILES];
  static int line[MAXFILES];
  static char *file_sp[MAXFILES];

#define USE_ENV
  //#define IDEM_PREFIX

#ifdef USE_ENV
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
#endif

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

  static Bool initialized = False;

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
	    if (!force) // added the 10/10/99
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

  static void
  push_file(const char *file)
  {
    fd = fopen(file, "r");

    if (!fd)
      error("%scannot open file '%s' for reading",
	    line_str(), file);

    pline = &line[fd_w];
    fd_sp[fd_w] = fd;
    file_sp[fd_w] = strdup(file);
    pfile = file;
    *pline = 1;
    fd_w++;
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

	push_file(file);
	return nexttoken(config);
      }

    return p;
  }

  static std::string
  get_conf()
  {
    const char *eyedbconf = getenv("EYEDBCONF");
    if (eyedbconf)
      return eyedbconf;

    return std::string(eyedblib::CompileBuiltin::getSysconfdir()) + "/eyedb/eyedb.conf";
  }

  void
  Config::init()
  {
    if (initialized) return;

    std::string eyedbconf = get_conf().c_str();

    if (eyedbconf.length() > 0)
      {
	new Config(eyedbconf.c_str());
	initialized = True;
	return;
      }

    new Config();

    initialized = True;
  }

  void
  Config::_release()
  {
    delete the_config;
  }

  void
  Config::addDefaults()
  {
    std::string libdir = eyedblib::CompileBuiltin::getLibdir();
    std::string localstatedir = eyedblib::CompileBuiltin::getLocalstatedir();
    std::string sysconfdir = eyedblib::CompileBuiltin::getSysconfdir();

    // Executables directory
    add( "bindir", eyedblib::CompileBuiltin::getBindir());

    // Bases directory
    add( "sv_datdir", (localstatedir + "/lib/eyedb/db").c_str());

    // pipes:
    add( "sv_pipedir", (localstatedir + "/lib/eyedb/pipes").c_str());

    // tmpdir
    add( "sv_tmpdir", (localstatedir + "/lib/eyedb/tmp").c_str());

    // sopath
    add( "sopath", (libdir + "/eyedb").c_str());

    // EYEDBDBM Database
    add( "sv_dbm", (localstatedir + "/lib/eyedb/db/dbmdb.dbs").c_str());

    // Server Parameters
    add( "sv_access_file", (sysconfdir + "/eyedb/Access").c_str());
    add( "sv_smdport", (localstatedir + "/lib/eyedb/pipes/eyedbsmd").c_str());

    // Port
    add( "port", (localstatedir + "/lib/eyedb/pipes/eyedbd").c_str());

    // Server Parameters
    add( "sv_port", (localstatedir + "/lib/eyedb/pipes/eyedbd").c_str());

    // OQL path
    add( "oqlpath", (libdir + "/eyedb/oql").c_str());

    // Hostname
    add( "host", "localhost");
  }

  Config::Config(const char *file)
  {
    if (!the_config)
      the_config = this;

    addDefaults();

    if (file) {
      add(file);
      init_done = 1;
      return;
    }

    init_done = 0;
  }

  Config::Config(const Config &config)
  {
    *this = config;
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

  void Config::add(const char *name, const char *value)
  {
    Item *item = new Item(name, value);
    list.insertObjectFirst(item);
  }

  void Config::setValue(const char *name, const char *value)
  {
    add((char *)name, (char *)value);
  }

  void Config::add(const char *file)
  {
    push_file(file);

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
	    add(name, value);
	    free(name);
	    free(value);
	    name = 0;
	    value = 0;
	    state = 0;
	    break;
	  }
      }
  }

  const char *
  Config::getValue(const char *name)
  {
#ifdef IDEM_PREFIX
    static const char prefix[] = "eyedb";
    static const int prefix_len = strlen(prefix);
#endif

#ifdef USE_ENV
    const char *s = getenv((std::string("EYEDB") + uppercase(name)).c_str());
    if (s)
      return s;
#endif

    LinkedListCursor c(list);
    Item *item;

    while (c.getNext((void *&)item))
      if (!strcasecmp(item->name, name)) {
	return item->value;
      }

#ifdef IDEM_PREFIX
    if (!strncasecmp(name, prefix, prefix_len))
      return getValue(&name[prefix_len]);
#endif

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

  Config *Config::getDefaultConfig()
  {
    return the_config;
  }

  void Config::setDefaultConfig(Config *config)
  {
    the_config = config;
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

  void
  Config::garbage()
  {
    LinkedListCursor c(list);
    Item *item;

    while (c.getNext((void *&)item))
      delete item;

    list.empty();
  }

  Config::~Config()
  {
    garbage();
  }

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

  const char *
  getConfigValue(const char *name)
  {
    return Config::getDefaultConfig()->getValue(name);
  }
}
