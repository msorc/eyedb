import pexpect
import sys

dbname='foo'
dspname='DEFAULT'
datname='foo.dat'

# create the data file
child = pexpect.spawn("eyedbadmin datcreate %s %s" % (dbname,datname))
r = child.expect(pexpect.EOF)
child.close()
# set current data file
command="eyedbadmin2 dataspace setcurdat %s %s %s" % (dbname,dspname,datname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


