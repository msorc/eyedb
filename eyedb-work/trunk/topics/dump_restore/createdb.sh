#!/bin/sh

db=dump_restore
dbdelete $db
dbcreate $db
odl -d $db -u schema.odl

