#!/bin/sh
#
# create.sh
#


db=bug_OQL_2

dbdelete $db
dbcreate $db

odl -d $db -u schema.odl
