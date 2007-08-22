from common import test_simple_command

command='eyedbadmin2 user sysaccess'
expected_output= [
"eyedbadmin user sysaccess \[--help\] <user> \['\+' combination of\] dbcreate\|adduser\|deleteuser\|setuserpasswd\|admin\|superuser\|no",
]

test_simple_command( command, expected_output, expected_status = 1)

