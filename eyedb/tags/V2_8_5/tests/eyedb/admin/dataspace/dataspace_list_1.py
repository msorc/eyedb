import pexpect
import sys

dbname = 'dataspace_test_db'

command="eyedbadmin2 dataspace list %s" % (dbname,)
child = pexpect.spawn(command)
r = child.expect( "Dataspace #0")
r = child.expect( "Name DEFAULT")
r = child.expect( "Datafile #0")
r = child.expect( "File")
r = child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type Logical")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


