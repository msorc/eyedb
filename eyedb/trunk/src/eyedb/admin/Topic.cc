
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

int TopicSet::perform(const std::string &prog, std::vector<std::string> &argv)
{
  if (argv.size() == 0) {
    return usage(prog);
  }

  std::string topic_name = argv[0];

  Topic *topic = 0;
  if (argv.size() >= 1) {
    topic = getTopic(topic_name);
    if (!topic) {
      return usage(prog);
    }
  }

  if (argv.size() == 1) {
    return topic->usage(prog, topic_name);
  }
  
  std::string cmd_name = argv[1];

  Command *cmd = topic->getCommand(cmd_name);
  if (!cmd) {
    return topic->usage(prog, topic_name);
  }

  // get rid of the two first arguments
  argv.erase(argv.begin());
  argv.erase(argv.begin());
  return cmd->perform(prog, argv);
}

int TopicSet::usage(const std::string &prog)
{
  std::cerr << "usage: " << prog << " <topic> <command> <options>\n\n";
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

Command *Topic::getCommand(const std::string &name)
{
  std::vector<Command *>::iterator begin = cmd_v.begin();
  std::vector<Command *>::iterator end = cmd_v.end();

  while (begin != end) {
    Command *cmd = *begin;
    if (cmd->getName() == name)
      return cmd;
    ++begin;
  }

  return 0;
}

int Topic::usage(const std::string &prog, const std::string &tname)
{
  std::cerr << "usage: " << prog << " " << tname << " <command> <options>\n\n";
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
