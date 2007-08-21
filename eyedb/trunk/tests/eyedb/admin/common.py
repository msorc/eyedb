import pexpect
import sys

def test_simple_command( command, expected_output):
    child = pexpect.spawn(command)
    index = child.expect([expected_output, pexpect.EOF])
    if index == 1:
        sys.exit(255)
        child.close()
    if child.exitstatus == 1:
        sys.exit(0)
        sys.exit(child.exitstatus)
