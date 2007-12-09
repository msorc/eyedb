/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
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

#include "eyedbconfig.h"
#include <eyedb/eyedb.h>

#include "Topic.h"

using namespace eyedb;

const std::string PROG_NAME = "eyedbadmin";

int main(int c_argc, char *c_argv[])
{
  eyedb::init(c_argc, c_argv);

  std::vector<std::string> argv;
  for (int n = 1; n < c_argc; n++) {
    argv.push_back(c_argv[n]);
  }

  Exception::setMode(Exception::ExceptionMode);

  try {
    Connection conn;
    return TopicSet::getInstance()->perform(conn, argv);
  }

  catch(Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }
}
