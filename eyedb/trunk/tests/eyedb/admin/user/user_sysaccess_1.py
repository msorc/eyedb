import pexpect
import sys

username='toto'

command="eyedbadmin2 user sysaccess %s dbcreate" % (username,)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
if child.exitstatus != 0:
    sys.exit(child.exitstatus)

command="eyedbadmin2 user sysaccess %s setuserpasswd" % (username,)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


