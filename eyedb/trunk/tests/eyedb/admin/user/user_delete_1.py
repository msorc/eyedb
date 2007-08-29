import pexpect
import sys

username='toto'
command="eyedbadmin2 user delete %s" % (username,)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


