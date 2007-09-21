import pexpect
import sys

dbname = 'database_test_db'
filedir = '/var/tmp'

# eyedbadmin database move [--help] [--dbfile=DBFILE] [--filedir=FILEDIR] DBNAME
command="eyedbadmin2 database move --filedir=%s %s" % (filedir,dbname,)
child = pexpect.spawn(command)
child.logfile = sys.stdout
child.expect(pexpect.EOF)
child.logfile = sys.stdout
child.close()
sys.exit(child.exitstatus)
