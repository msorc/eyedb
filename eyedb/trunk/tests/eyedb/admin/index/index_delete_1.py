import pexpect
import sys

dbname = 'index_test_db'
attribute = 'Person.lastName'

# delete the index
command="eyedbadmin2 index delete %s %s" % (dbname,attribute)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect("Deleting index %s" % (attribute,))
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
