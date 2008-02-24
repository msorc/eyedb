#!/bin/sh

for i in $*
do
  cmd=$i

  echo "

// in the .h
CMDCLASS_GETOPT(DBSRenameCmd, \"name\");

// in the .cpp
void $cmd::init()
{
  std::vector<Option> opts;

  opts.push_back(HELP_OPT);

  // [...]

  getopt = new GetOpt(getExtName(), opts);
}

int $cmd::usage()
{
  getopt->usage(\"\", \"\");

  // std::cerr << \"Extra arg\\n\";

  return 1;
}

int $cmd::help()
{
  stdhelp();

  //getopt->displayOpt(\"Extra arg, \"Description\");

  return 1;
}

int $cmd::perform(eyedb::Connection &conn, std::vector<std::string> &argv)
{
  bool r = getopt->parse(PROG_NAME, argv);

  GetOpt::Map &map = getopt->getMap();

  if (map.find(\"help\") != map.end()) {
    return help();
  }

  if (!r) {
    return usage();
  }

  /*
  if (argv.size() != N) { // N : number of extra args (could be 0)
    return usage();
  }
  */

  conn.open();

  // [...]

  return 0;
}
"

done
