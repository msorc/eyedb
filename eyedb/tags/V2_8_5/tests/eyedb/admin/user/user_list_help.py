from common import test_simple_command

command='eyedbadmin2 user list --help'
expected_output=[
'eyedbadmin user list \[--help\] \[USER\]',
'',
'  --help Displays the current help',
'  USER   User name',
]
test_simple_command( command, expected_output, expected_status=1)
