import pexpect
import sys

dbname='foo'
dspname='DEFAULT'

command="eyedbadmin2 dataspace getcurdat %s %s" % (dbname,dspname)
child = pexpect.spawn(command)
r = child.expect( "Datafile")
r = child.expect( "Name *DEFAULT")
r = child.expect( "Dataspace")
r = child.expect( "File *foo.dat")
r = child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type *Logical")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


