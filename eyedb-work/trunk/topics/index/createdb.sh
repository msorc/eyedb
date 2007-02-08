#!/bin/sh

db=index_test
dbdelete $db
dbcreate $db
odl -d $db -u schema.odl

