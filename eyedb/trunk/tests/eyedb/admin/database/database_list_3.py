import pexpect
import sys

dbname = 'new_database_test_db'

command="eyedbadmin2 database list %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect( "Database Name")
child.expect( " *%s" % (dbname,))
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


