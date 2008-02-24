from common import test_simple_command
import sys

dbname = 'new_database_test_db'
copy_dbname = 'copy_database_test_db'

command="eyedbadmin2 database copy %s %s" % (dbname, copy_dbname)
test_simple_command( command)
sys.exit( 0)
