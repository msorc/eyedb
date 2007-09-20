import pexpect
import sys

dbname = 'datafile_test_db'
new_datafile='new_test.dat'
name='test'

command="eyedbadmin2 datafile move --filedir=/var/tmp %s %s %s" % (dbname,name,new_datafile)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.close()
sys.exit(child.exitstatus)


