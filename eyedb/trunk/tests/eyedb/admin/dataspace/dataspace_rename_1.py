import pexpect
import sys

dbname='foo'
dspname='bar'
newdspname='bur'

command="eyedbadmin2 dataspace rename %s %s %s" % (dbname,dspname,newdspname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


