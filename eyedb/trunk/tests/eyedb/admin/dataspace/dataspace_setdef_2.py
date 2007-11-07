import pexpect
import sys

dbname = 'dataspace_test_db'
dspname='DEFAULT'

command="eyedbadmin2 dataspace setdef %s %s" % (dbname,dspname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


