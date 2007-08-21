import pexpect
import sys

def test_simple_command( command, expected_output, expected_status = 0):
    child = pexpect.spawn(command)
    for o in expected_output:
        index = child.expect([o, pexpect.EOF])
        if index == 1:
            break
    if index == 1:
        sys.exit(255)
    child.close()
    if child.exitstatus == expected_status:
        sys.exit(0)
    else:
        sys.exit(child.exitstatus)
