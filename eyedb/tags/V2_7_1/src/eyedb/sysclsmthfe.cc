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


#include <eyedb/eyedb.h>

using namespace eyedb;

// To implement and use user methods, perform the following operations
/*

#1. Copy the skeleton file
cp sysclsmthfe-skel.cc sysclsmthfe.cc

#2. Implement the user methods in sysclsmthfe.cc using a text editor

#3. Compile the shared library using GNU make
make -f Makefile.syscls all

#4. Copy the shared library to the eyedb loadable library directory
cp sysclsmthfe-2.7.0.so $EYEDBROOT/$EYEDBARCH/etc/so/.

#5. Change the file access mode
chmod a+r $EYEDBROOT/$EYEDBARCH/etc/so/sysclsmthfe-2.7.0.so

*/

static Bool __syscls_init = False;

#define _packageInit(DB) \
 \
  if (!__syscls_init) { \
    syscls::init(); \
    __syscls_init = True; \
  } \
 \
  if (!(DB)->getUserData("eyedb:syscls")) { \
     Status s = sysclsDatabase::checkSchema((DB)->getSchema()); \
     if (s) return s; \
     sysclsDatabase::setConsApp(DB); \
     (DB)->setUserData("eyedb:syscls", (void *)1); \
  }

