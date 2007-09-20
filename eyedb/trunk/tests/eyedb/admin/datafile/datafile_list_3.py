import pexpect
import sys

dbname = 'datafile_test_db'
new_datafile='new_test.dat'
name='test'

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

child.expect( "Datafile #1")
child.expect( "Name *%s" % (name,))
child.expect( "File .*%s" % (new_datafile,))
child.expect( "Maxsize")
child.expect( "Slotsize")
child.expect( "Oid Type *Logical")

child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


