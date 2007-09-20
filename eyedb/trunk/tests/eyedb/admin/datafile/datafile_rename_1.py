import pexpect
import sys

dbname = 'datafile_test_db'
name='test'
new_name='new_test'

command="eyedbadmin2 datafile rename %s %s %s" % (dbname,name,new_name)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


