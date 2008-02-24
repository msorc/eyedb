import pexpect
import sys

dbname = 'datafile_test_db'

command="eyedbadmin2 datafile list %s" % (dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect( "Datafile #0")
child.expect( "Name *DEFAULT")
child.expect( "Dataspace #0 DEFAULT")
child.expect( "File *%s.dat" % (dbname,))
child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type *Logical")
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


