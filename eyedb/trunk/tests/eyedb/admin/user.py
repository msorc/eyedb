#!/usr/bin/env python
import pexpect
import sys
from common import test_simple_command

test_simple_command( 'eyedbadmin2 user delete', 'eyedbadmin user delete \[--help\] <user>')
