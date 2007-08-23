import pexpect
import sys

dbname='foo'
command="eyedbadmin2 dataspace getdef %s" % (dbname,)
child = pexpect.spawn(command)
r = child.expect( "Dataspace #1")
r = child.expect( "Name bur")
r = child.expect( "Datafile")
r = child.expect( "File")
r = child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type Logical")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


