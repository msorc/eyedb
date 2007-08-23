import pexpect
import sys

dbname='foo'
dspname='bar'
datname='bar.dat'
command="eyedbadmin2 dataspace create %s %s %s" % (dbname,dspname,datname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


