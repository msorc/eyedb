import pexpect
import sys

dbname = 'dataspace_test_db'
dspname='bar'
datname='bar.dat'

# create the data file
child = pexpect.spawn("eyedbadmin datcreate %s %s" % (dbname,datname))
r = child.expect(pexpect.EOF)
child.close()
# create the dataspace
command="eyedbadmin2 dataspace create %s %s %s" % (dbname,dspname,datname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


