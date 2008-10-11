/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-2008 SYSRA
   
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

#include <eyedb/utils.h>

using namespace eyedb;
// To implement and use user methods, perform the following operations
/*

#1. Copy the skeleton file
cp utilsmthbe-skel.cc utilsmthbe.cc

#2. Implement the user methods in utilsmthbe.cc using a text editor

#3. Compile the shared library using GNU make
make -f Makefile.utils all

#4. Copy the shared library to the eyedb loadable library directory
cp utilsmthbe-2.5.7.so $EYEDBROOT/$EYEDBARCH/etc/so/.

#5. Change the file access mode
chmod a+r $EYEDBROOT/$EYEDBARCH/etc/so/utilsmthbe-2.5.7.so

*/

static Bool __utils_init = False;
static Database *__utils_db = 0;

#define PACK_INIT(DB) if (!__utils_init) {utils::init(); __utils_init = True;} if (__utils_db != DB) {__utils_db = DB; Status s = utilsDatabase::checkSchema((DB)->getSchema()); if (s) return s;}

