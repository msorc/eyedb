import pexpect
import sys

dbname = 'dataspace_test_db'
dspname='XYZZY'

command="eyedbadmin2 dataspace list %s %s " % (dbname, dspname)
child = pexpect.spawn(command)
child.expect( "eyedb error: dataspace %s not found in database %s" % (dspname, dbname))
r = child.expect(pexpect.EOF)
child.close()
if child.exitstatus == 1:
    sys.exit(0)
sys.exit(child.exitstatus)


