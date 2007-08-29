import pexpect
import sys

def test_simple_command( command, expected_output, expected_status = 0):
    child = pexpect.spawn(command)
    child.logfile = sys.stdout
    for o in expected_output:
        child.expect(o)
    child.expect(pexpect.EOF)
    child.close()
    if child.exitstatus == expected_status:
        sys.exit(0)
    sys.exit(child.exitstatus)
