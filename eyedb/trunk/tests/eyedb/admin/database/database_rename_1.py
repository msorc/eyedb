import pexpect
import sys

dbname = 'database_test_db'
new_dbname = 'new_database_test_db'

command="eyedbadmin2 database rename %s %s" % (dbname, new_dbname)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.logfile = sys.stdout
child.close()
sys.exit(child.exitstatus)
