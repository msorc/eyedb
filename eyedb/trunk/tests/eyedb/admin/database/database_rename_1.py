from common import test_simple_command
import sys

dbname = 'database_test_db'
new_dbname = 'new_database_test_db'

command="eyedbadmin2 database rename %s %s" % (dbname, new_dbname)
test_simple_command( command)
sys.exit( 0)
