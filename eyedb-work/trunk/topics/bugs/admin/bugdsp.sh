#!/bin/bash
#
# bugdsp.sh
#

db=foo4

eyedb_dbdelete $db

eyedb_dbcreate $db

eyedbadmin dataspace list $db
eyedbadmin datafile create --size=200 $db ${db}02.dat
eyedbadmin datafile create --size=200 $db ${db}03.dat
eyedbadmin datafile create --size=200 $db ${db}04.dat
eyedbadmin datafile create --size=200 $db ${db}05.dat
eyedbadmin dataspace create $db DSP_2 1
eyedbadmin dataspace update $db DEFAULT 2 --migrate-orphan=DSP_2

# this command or any command failed !
eyedboql -d $db -w
