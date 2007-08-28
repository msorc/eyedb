import pexpect
import sys

dbname = 'index_test_db'
attribute = 'Person.lastName'

# create the index
command="eyedbadmin2 index create %s %s" % (dbname,attribute)
child = pexpect.spawn(command)
r = child.expect("Creating hash index on %s" % (attribute,))
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
