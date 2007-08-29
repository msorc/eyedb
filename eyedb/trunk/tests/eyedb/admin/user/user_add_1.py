import pexpect
import sys

username='toto'
password='titi'
command="eyedbadmin2 user add %s" % (username,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect("%s password: " % (username,))
#print "expect 1 returned %d" % r
child.sendline( password)
r = child.expect("retype %s password: " % (username,))
#print "expect 2 returned %d" % r
child.sendline( password)
r = child.expect(pexpect.EOF)
#print "expect 3 returned %d" % r
child.close()
#print child.exitstatus
sys.exit(child.exitstatus)


