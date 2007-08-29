import pexpect
import sys

username='toto'
dbname='foo'

command="eyedbadmin2 user dbaccess %s %s rwx" % (username,dbname)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)

