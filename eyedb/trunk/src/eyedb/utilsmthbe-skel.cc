
/*
 * EyeDB Version 2.8.8 Copyright (c) 1995-2006 SYSRA
 *
 * File 'utilsmthbe-skel.cc'
 *
 * Package Name 'utils'
 *
 * Generated by eyedbodl at Mon Apr 12 16:51:38 2010
 *
 */

#include <eyedb/eyedb.h>

#include "utils.h"

using namespace eyedb;

// To implement and use user methods, perform the following operations
/*

#1. Copy the skeleton file
cp utilsmthbe-skel.cc utilsmthbe.cc

#2. Implement the user methods in utilsmthbe.cc using a text editor

#3. Compile the shared library

#4. Copy the shared library to the eyedb loadable library directory
cp utilsmthbe-2.8.8.so <eyedbinstalldir>/lib/eyedb

#5. Change the file access mode
chmod a+r <eyedbinstalldir>/lib/eyedb/utilsmthbe-2.8.8.so

*/

static Bool __utils_init = False;

#define _packageInit(DB) \
 \
  if (!__utils_init) { \
    utils::init(); \
    __utils_init = True; \
  } \
 \
  if (!(DB)->getUserData("eyedb:utils")) { \
     Status s = utilsDatabase::checkSchema((DB)->getSchema()); \
     if (s) return s; \
     utilsDatabase::setConsApp(DB); \
     (DB)->setUserData("eyedb:utils", (void *)1); \
  }

