import pexpect
import sys

dbname = 'database_test_db'

command="eyedbadmin2 database create %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.logfile = sys.stdout
child.close()
sys.exit(child.exitstatus)
