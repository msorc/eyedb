from eyedb.test.command import run_simple_command
import os

username='toto'
dbname='foo'

command="%s/eyedbadmin user dbaccess %s %s rwx" % (os.environ['bindir'], username, dbname)
run_simple_command( command)
