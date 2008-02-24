import pexpect
import sys

dbname = 'datafile_test_db'
datafile='/var/tmp/new_test.dat'

command="eyedbadmin2 datafile delete %s %s" % (dbname,datafile)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


