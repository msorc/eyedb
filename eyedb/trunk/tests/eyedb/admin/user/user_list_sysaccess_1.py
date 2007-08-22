import pexpect
import sys

username='toto'
command="eyedbadmin2 user list"
child = pexpect.spawn(command)
child.expect("name .*: \"%s\"\r\n" % (username,))
child.expect("sysaccess .*: NO_SYSACCESS_MODE\r\n")
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


