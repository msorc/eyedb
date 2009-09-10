
/*
 * EyeDB Version 2.8.8 Copyright (c) 1995-2006 SYSRA
 *
 * File 'oqlctbmthbe-skel.cc'
 *
 * Package Name 'oqlctb'
 *
 * Generated by eyedbodl at Thu Sep 10 15:17:19 2009
 *
 */

#include <eyedb/eyedb.h>

#include "oqlctb.h"

using namespace eyedb;

// To implement and use user methods, perform the following operations
/*

#1. Copy the skeleton file
cp oqlctbmthbe-skel.cc oqlctbmthbe.cc

#2. Implement the user methods in oqlctbmthbe.cc using a text editor

#3. Compile the shared library

#4. Copy the shared library to the eyedb loadable library directory
cp oqlctbmthbe-2.8.8.so <eyedbinstalldir>/lib/eyedb

#5. Change the file access mode
chmod a+r <eyedbinstalldir>/lib/eyedb/oqlctbmthbe-2.8.8.so

*/

static Bool __oqlctb_init = False;

#define _packageInit(DB) \
 \
  if (!__oqlctb_init) { \
    oqlctb::init(); \
    __oqlctb_init = True; \
  } \
 \
  if (!(DB)->getUserData("eyedb:oqlctb")) { \
     Status s = oqlctbDatabase::checkSchema((DB)->getSchema()); \
     if (s) return s; \
     oqlctbDatabase::setConsApp(DB); \
     (DB)->setUserData("eyedb:oqlctb", (void *)1); \
  }

