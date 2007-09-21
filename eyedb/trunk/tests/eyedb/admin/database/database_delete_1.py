import pexpect
import sys

dbname = 'new_database_test_db'

command="eyedbadmin2 database delete %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.logfile = sys.stdout
child.close()
sys.exit(child.exitstatus)
