#!/usr/bin/env python
from common import test_simple_command

test_simple_command( 'eyedbadmin2 user add', 'eyedbadmin user delete \[--help\] <user>')

test_simple_command( 'eyedbadmin2 user delete', 'eyedbadmin user delete \[--help\] <user>')
