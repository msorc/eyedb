import pexpect
import sys

def database_create( dbname):
    # test if database exists
    dblist = "eyedbadmin2 database list %s" % (dbname,)
    child = pexpect.spawn( dblist)
    dblistmsg = "Database '%s' not found" % (dbname,)
    r = child.expect([dblistmsg, pexpect.EOF])
    # if it exists, delete it
    if r == 1:
        dbdelete = "eyedbadmin2 database delete %s" % (dbname,)
        (command_output, exitstatus) = pexpect.run (dbdelete, withexitstatus=1)
        if exitstatus != 0:
            sys.exit(exitstatus)
    # create the database
    dbcreate = "eyedbadmin2 database create %s" % (dbname,)
    (command_output, exitstatus) = pexpect.run (dbcreate, withexitstatus=1)
    if exitstatus != 0:
        sys.exit(exitstatus)
