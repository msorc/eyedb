from common import test_simple_command

command='eyedbadmin2 user add'
expected_output= [
'eyedbadmin user add \[--help\] \[--unix\] \[--strict-unix\] \[USER \[PASSWORD\]]',
'',
'  --help        Displays the current help',
'  --unix        ',
'  --strict-unix ',
'  USER          User name',
'  PASSWORD      Password for specified user'
]

test_simple_command( command, expected_output, expected_status = 1)

