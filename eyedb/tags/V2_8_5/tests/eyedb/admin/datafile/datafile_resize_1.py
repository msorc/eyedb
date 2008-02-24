import pexpect
import sys

dbname = 'datafile_test_db'
name='new_test'
new_size=4096

command="eyedbadmin2 datafile resize %s %s %s" % (dbname,name,new_size)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


