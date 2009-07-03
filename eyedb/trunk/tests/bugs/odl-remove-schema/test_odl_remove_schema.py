import os
from eyedb.test.command import run_simple_command

database='bugs_odl_remove_schema'

command="%s/eyedbodl -u -d %s --rmsch" % (os.environ['bindir'], database)
run_simple_command( command)
