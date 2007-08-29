import pexpect
import sys

command="eyedbadmin2 user list"
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect("name .*: .* \[strict unix user\]\r\n")
child.expect("sysaccess .*: SUPERUSER_SYSACCESS_MODE\r\n")
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


