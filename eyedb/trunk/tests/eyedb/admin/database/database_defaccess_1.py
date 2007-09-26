from common import test_simple_command
import sys

dbname = 'new_database_test_db'

command="eyedbadmin2 database defaccess %s r" % (dbname,)
test_simple_command( command)
sys.exit( 0)
