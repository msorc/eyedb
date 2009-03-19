#!/bin/bash
eyedbuser=toto
eyedbpasswd=123

eyedbadmin user add $eyedbuser $eyedbpasswd

# This commands exits with error:
#set user system access error: invalid database access mode 0x700
eyedbadmin user sysaccess $eyedbuser dbcreate+adduser+deleteuser

# See DBM_Database.cc line 996 #define check_sysaccess(s, mode)

# fixed 2009-03-19 by EV: dbcreate+adduser+deleteuser no granted anymore by eyedbadmin (help has been updated)

