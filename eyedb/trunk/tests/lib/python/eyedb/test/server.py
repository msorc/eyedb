import pexpect
import sys
import os
from eyedb.test.command import run_command

def server_status():
    status_cmd = [ "%s/eyedbctl" % (os.environ['sbindir'],), "status" ]
    return run_command( status_cmd, do_exit=False)

def server_start():
    if  server_status() != 0:
        start_cmd = ["%s/eyedbctl" % (os.environ['sbindir'],), "start" ]
        status = run_command( start_cmd, do_exit=False)
        return status
    return 0

def server_stop():
    if server_status() == 0:
        stop_cmd = ["%s/eyedbctl" % (os.environ['sbindir'],), "stop" ]
#        stop_cmd = "%s/eyedbctl stop" % (os.environ['sbindir'],)
        return run_command( stop_cmd, do_exit=False)
    return 0
