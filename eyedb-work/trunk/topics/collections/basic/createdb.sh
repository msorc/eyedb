#!/bin/sh

db=collbasic
dbdelete $db
dbcreate $db
odl -d $db -u schema.odl

