
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

#include "DBTopic.h"
#include "DSPTopic.h"
#include "DTFTopic.h"

TopicSet *TopicSet::instance;

TopicSet::TopicSet()
{
  addTopic(new DBTopic());
  addTopic(new DTFTopic());
  addTopic(new DSPTopic());
}

int TopicSet::perform(int argc, char *argv[])
{
  std::string prog_name = argv[0];

  if (argc == 1) {
    return usage(prog_name);
  }

  std::string topic_name = argv[1];

  if (argc == 2) {
    Topic *topic = getTopic(topic_name);
    if (!topic) {
      return usage(prog_name);
    }

    return topic->usage(prog_name, topic_name);
  }
}

int TopicSet::usage(const std::string &prog_name)
{
  std::cerr << "usage: " << prog_name << " <topic> <command> <options>\n\n";
  std::cerr << "where <topic> is one of the following:\n";

  std::vector<Topic *>::iterator begin = topic_v.begin();
  std::vector<Topic *>::iterator end = topic_v.end();

  while (begin != end) {
    Topic *topic = *begin;
    topic->display(std::cerr);
    std::cerr << '\n';
    ++begin;
  }

  return 1;
}

Topic *TopicSet::getTopic(const std::string &name)
{
  std::vector<Topic *>::iterator begin = topic_v.begin();
  std::vector<Topic *>::iterator end = topic_v.end();

  while (begin != end) {
    Topic *topic = *begin;
    if (topic->isTopic(name))
      return topic;
    ++begin;
  }
}

void Topic::display(std::ostream &os)
{
  os << name;
  std::vector<std::string>::iterator begin = alias_v.begin();
  std::vector<std::string>::iterator end = alias_v.end();

  while (begin != end) {
    os << '|' << (*begin);
    ++begin;
  }
}

bool Topic::isTopic(const std::string &name) const
{
  if (this->name == name)
    return true;

  std::vector<std::string>::const_iterator begin = alias_v.begin();
  std::vector<std::string>::const_iterator end = alias_v.end();

  while (begin != end) {
    if (name == *begin)
      return true;
    ++begin;
  }

  return false;
}

int Topic::usage(const std::string &prog_name, const std::string &tname)
{
  std::cerr << "usage: " << prog_name << " " << tname << " <command> <options>\n\n";
  std::cerr << "where <command> is one of the following:\n";

  std::vector<Command *>::iterator begin = cmd_v.begin();
  std::vector<Command *>::iterator end = cmd_v.end();

  while (begin != end) {
    Command *cmd = *begin;
    std::cerr << cmd->getName() << '\n';
    ++begin;
  }

  return 1;
}
