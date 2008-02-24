import pexpect
import sys

username='toto'
command="eyedbadmin2 user list %s" % (username,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect("eyedbadmin: user %s not found\r\n" % (username,))
child.expect(pexpect.EOF)
child.close()
if child.exitstatus == 1:
    sys.exit(0)
sys.exit(child.exitstatus)

