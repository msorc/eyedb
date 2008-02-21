
/*
 * EyeDB Version 2.8.4 Copyright (c) 1995-2006 SYSRA
 *
 * File 'DBMmthfe-skel.cc'
 *
 * Package Name 'DBM'
 *
 * Generated by eyedbodl at Fri Jan 18 22:13:24 2008
 *
 */

#include <eyedb/eyedb.h>

#include "DBM.h"

using namespace eyedb;

// To implement and use user methods, perform the following operations
/*

#1. Copy the skeleton file
cp DBMmthfe-skel.cc DBMmthfe.cc

#2. Implement the user methods in DBMmthfe.cc using a text editor

#3. Compile the shared library

#4. Copy the shared library to the eyedb loadable library directory
cp DBMmthfe-2.8.4.so <eyedbinstalldir>/lib/eyedb

#5. Change the file access mode
chmod a+r <eyedbinstalldir>/lib/eyedb/DBMmthfe-2.8.4.so

*/

static Bool __DBM_init = False;

#define _packageInit(DB) \
 \
  if (!__DBM_init) { \
    DBM::init(); \
    __DBM_init = True; \
  } \
 \
  if (!(DB)->getUserData("eyedb:DBM")) { \
     Status s = DBMDatabase::checkSchema((DB)->getSchema()); \
     if (s) return s; \
     DBMDatabase::setConsApp(DB); \
     (DB)->setUserData("eyedb:DBM", (void *)1); \
  }

