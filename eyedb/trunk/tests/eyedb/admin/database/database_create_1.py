from common import test_simple_command
import sys

dbname = 'database_test_db'

command="eyedbadmin2 database create %s" % (dbname,)
test_simple_command( command)
sys.exit( 0)
