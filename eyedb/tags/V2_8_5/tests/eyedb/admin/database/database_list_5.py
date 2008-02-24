import pexpect
import sys

dbname = 'new_database_test_db'

command="eyedbadmin2 database list %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect( "Default Access")
child.expect( " *READ_DBACCESS_MODE")
child.expect(pexpect.EOF)
child.close()
if child.exitstatus != 0:
    sys.exit(child.exitstatus)

