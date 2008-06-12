from eyedb.test.command import run_simple_command
import sys
import os

dbname = 'dataspace_test_db'
dspname='DEFAULT'
datname='foo.dat'

# create the data file
command = "%s/eyedbadmin datcreate %s %s" % (os.environ['bindir'], dbname, datname)
status = run_simple_command( command, do_exit = False)
if status != 0:
    sys.exit( status)

# set current data file
command="%s/eyedbadmin dataspace setcurdat %s %s %s" % (os.environ['bindir'], dbname, dspname, datname)
run_simple_command( command)
