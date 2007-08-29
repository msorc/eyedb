import pexpect
import sys

dbname = 'index_test_db'
attribute = 'Person.age'

# create the index
command="eyedbadmin2 index create %s %s" % (dbname,attribute)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect("Creating btree index on %s" % (attribute,))
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
