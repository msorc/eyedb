import pexpect
import os
import sys
from common import create_eyedb_database

dbname = 'index_test_db'

create_eyedb_database( dbname)

# fill an ODL schema into it
odl = "eyedbodl -u -d %s --nocpp %s/person.odl" % (dbname, os.environ['srcdir'],)
child = pexpect.spawn (odl)
child.logfile = sys.stdout
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
                                
