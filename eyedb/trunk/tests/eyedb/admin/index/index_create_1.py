import pexpect
import sys

dbname='foo'
dspname='bar'
datname='bar.dat'

# create the index
# eyedbidxadmin create index_test_db2 Person.firstName hash
command="eyedbadmin2 index create %s %s %s" % (dbname,dspname,datname)
child = pexpect.spawn(command)
r = child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


