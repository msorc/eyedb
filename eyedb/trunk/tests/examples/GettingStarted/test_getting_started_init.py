import pexpect
import sys
import os
from eyedb.test.server import server_start, server_stop
from eyedb.test.command import run_command

print '... Starting server'
status = server_start()
if status != 0:
    sys.exit(status)
print '... server started'
    
print '... Running init script'
# run the init script
initscript= ["sh",
             "-x",
             "%s/examples/GettingStarted/init.sh" % (os.environ['top_builddir'],), 
             "%s/examples/GettingStarted/schema.odl" % (os.environ['top_srcdir'],)]
status = run_command( initscript, do_exit=False)
if status != 0:
    sys.exit(status)
print '... init script done'

print '... Stopping server'
status = server_stop()
if status != 0:
    sys.exit(status)
print '... server stopped'

sys.exit(0)

