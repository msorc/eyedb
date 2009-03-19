#!/bin/bash

db=zob

eyedbadmin database create $db
eyedbadmin datafile list $db
eyedbadmin datafile create $db $db-A.dat
eyedbadmin datafile list $db
eyedbadmin datafile rename $db 1 $db-A
eyedbadmin datafile list $db
eyedbadmin dataspace list $db
eyedbadmin dataspace update $db DEFAULT 1
# next command fails with:
#database load object error: loading object 3.2.446364:oid: cannot find dataspace for datafile 0
eyedbadmin datafile list $db

