from eyedb.test.command import run_simple_command
import os

dbname = 'dataspace_test_db'
dspname='DEFAULT'

command="%s/eyedbadmin database setdefdsp %s %s" % (os.environ['bindir'], dbname, dspname)
run_simple_command( command)
