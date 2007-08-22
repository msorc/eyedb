import pexpect
import sys

username='toto'
password='titi'
newpassword='tata'

command="eyedbadmin2 user passwd %s " % (username,)
child = pexpect.spawn(command)
r = child.expect("user old password: ")
child.sendline( password)
r = child.expect("user new password: ")
child.sendline( newpassword)
r = child.expect("retype user new password: ")
child.sendline( newpassword)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
