from common import test_simple_command

command='eyedbadmin2 user dbaccess'
expected_output= [
"eyedbadmin user dbaccess \[--help\] <user> <dbname> r\|rw\|rx\|rwx\|admin\|no",
]

test_simple_command( command, expected_output, expected_status = 1)

