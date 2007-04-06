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

#ifndef _EYEDB_ADMIN_COMMAND_H
#define _EYEDB_ADMIN_COMMAND_H

#include <string>
#include <vector>
#include <iostream>

class Topic;

class Command {

protected:
  std::string name;
  Topic *topic;

  Command(Topic *topic, const std::string &name) : topic(topic), name(name) { }

public:

  const std::string &getName() const {return name;}
  bool isCommand(std::string name) const;

  virtual void usage() = 0;
  virtual void help() = 0;
  virtual int perform() = 0;
};

// to be moved to Command.h
#define CMDCLASS(CLS, NAME) \
\
class CLS : public Command { \
\
public: \
  CLS(Topic *topic) : Command(topic, NAME) { } \
\
  virtual void usage() { std::cout << "usage: " << NAME << '\n'; } \
  virtual void help() { std::cout << "help: " << NAME << '\n'; } \
  virtual int perform() { std::cout << "perform: " << NAME << '\n'; return 0;} \
}

#endif
