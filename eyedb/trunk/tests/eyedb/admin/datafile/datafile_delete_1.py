import pexpect
import sys

dbname='foo'
dspname='bur'

command="eyedbadmin2 dataspace delete %s %s" % (dbname,dspname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


