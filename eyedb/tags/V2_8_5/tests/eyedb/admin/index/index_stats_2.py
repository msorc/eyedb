import pexpect
import sys

dbname = 'index_test_db'
attribute_1 = 'Person.firstName'
attribute_2 = 'Person.lastName'
attribute_3 = 'Person.age'
attribute_4 = 'Person.id'

# get the index statistics
command="eyedbadmin2 index stats %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect("Index on %s" % (attribute_1,))
r = child.expect("Propagation: on")
r = child.expect("Type: Hash")
r = child.expect("Index on %s" % (attribute_2,))
r = child.expect("Propagation: off")
r = child.expect("Type: Hash")
r = child.expect("Index on %s" % (attribute_3,))
r = child.expect("Propagation: on")
r = child.expect("Type: BTree")
r = child.expect("Index on %s" % (attribute_4,))
r = child.expect("Propagation: on")
r = child.expect("Type: Hash")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)
