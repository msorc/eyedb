
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
#include "eyedb/DBM_Database.h"
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <eyedblib/butils.h>

#include "GetOpt.h"

#include "IDXTopic.h"

IDXTopic::IDXTopic() : Topic("index")
{
  addAlias("idx");

  addCommand(new IDXCreateCmd(this));
  addCommand(new IDXDeleteCmd(this));
  addCommand(new IDXUpdateCmd(this));
  addCommand(new IDXListCmd(this));
  addCommand(new IDXStatsCmd(this));
  addCommand(new IDXSimulateCmd(this));
}

#if 0
"create <dbname> [--check] {<attrpath> [hash|btree [<hints>|\"\" [propagate=on|propagate=off |\"\"]]]}\n"
"update <dbname> [--check] {<attrpath> [hash|btree [<hints>|\"\"]] [propagate=on|propagate=off |\"\"]}\n"
"simulate <dbname> [--full] [--fmt=<fmt>] {<attrpath> hash|btree [<hints>]}\n"
"list <dbname> [--full] {[<attrpath>|<classname>|--all]}\n"
"stats <dbname> [--full] [--fmt=<fmt>] {[<attrpath>|<classname>|--all]}\n"
"delete <dbname> {<attrpath>}\n"
#endif
