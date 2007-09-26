from common import test_simple_command
import sys

dbname = 'database_test_db'
filedir = '/var/tmp'

command="eyedbadmin2 database move --filedir=%s %s" % (filedir,dbname,)
test_simple_command( command)
sys.exit( 0)
