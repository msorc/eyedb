import pexpect
import sys

dbname = 'index_test_db'
attribute = 'Person.firstName'

# create the index
command="eyedbadmin2 index list %s %s" % (dbname,attribute)
child = pexpect.spawn(command)
r = child.expect("hash index on %s" % (attribute,))
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
