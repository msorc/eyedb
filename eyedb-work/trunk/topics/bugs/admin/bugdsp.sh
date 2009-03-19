#!/bin/bash
#
# bugdsp.sh
#

db=foo3

eyedb_dbdelete $db

eyedb_dbcreate $db

eyedbadmin dataspace list $db
eyedbadmin datafile create --size=200 $db ${db}02.dat
eyedbadmin datafile create --size=200 $db ${db}03.dat
eyedbadmin dataspace update $db 0 1 2

# this command or any command failed !
eyedboql -d $db -w
