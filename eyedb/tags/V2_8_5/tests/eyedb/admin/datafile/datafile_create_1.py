import pexpect
import sys

dbname = 'datafile_test_db'
datafile='test.dat'
name='test'

# create the data file
command="eyedbadmin2 datafile create --name=%s %s %s" % (name,dbname,datafile)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.logfile = sys.stdout
child.close()
sys.exit(child.exitstatus)


