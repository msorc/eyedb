#!/bin/sh

db=virtattr
dbdelete $db
dbcreate $db
odl -d $db -u schema.odl

