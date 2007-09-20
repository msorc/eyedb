import pexpect
import sys

import pexpect
import sys

dbname = 'datafile_test_db'

command="eyedbadmin2 datafile list %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
r = child.expect( "Datafile #0")
r = child.expect( "Name *DEFAULT")
r = child.expect( "Dataspace #0 DEFAULT")
r = child.expect( "File *%s.dat" % (dbname,))
r = child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type *Logical")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


