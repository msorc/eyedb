#!/bin/sh
#
# create.sh
#


db=bug_OQL_1

dbdelete $db
dbcreate $db

odl -d $db -u schema.odl
