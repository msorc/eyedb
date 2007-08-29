import pexpect
import sys

username='toto'
command="eyedbadmin2 user list"
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect("name .*: \"%s\"\r\n" % (username,))
child.expect("sysaccess .*: SET_USER_PASSWD_SYSACCESS_MODE\r\n")
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


