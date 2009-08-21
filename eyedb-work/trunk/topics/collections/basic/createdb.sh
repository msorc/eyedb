#!/bin/sh

db=collbasic
eyedb_dbdelete $db
eyedb_dbcreate $db
eyedbodl -d $db -u schema.odl

