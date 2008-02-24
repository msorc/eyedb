from common import test_simple_command

command='eyedbadmin2 user delete'
expected_output=['eyedbadmin user delete \[--help\] USER']
test_simple_command( command, expected_output, expected_status=1)
