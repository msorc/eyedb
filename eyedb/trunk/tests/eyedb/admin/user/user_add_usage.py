from common import test_simple_command

command='eyedbadmin2 user add'
expected_output= ['eyedbadmin user add \[--help\] \[--unix\] \[--strict-unix\] \[<user> \[<passwd>\]]',
                  '',
                  '  --help        Displays the current help',
                  '  --unix        ',
                  '  --strict-unix ',
                  '  <user>        User name',
                  '  <passwd>      Password for specified user']

test_simple_command( command, expected_output, expected_status = 1)

