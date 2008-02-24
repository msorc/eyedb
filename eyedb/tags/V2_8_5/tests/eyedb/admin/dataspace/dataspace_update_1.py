import pexpect
import sys

dbname = 'dataspace_test_db'
dspname='bar'
datname='joe.dat'

# create the data file
child = pexpect.spawn("eyedbadmin datcreate %s %s" % (dbname,datname))
r = child.expect(pexpect.EOF)
child.close()
# update the dataspace
command="eyedbadmin2 dataspace update %s %s %s" % (dbname,dspname,datname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


