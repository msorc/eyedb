#!/bin.sh
#
# 

db=objgarb_test
eyedbdbdelete $db
eyedbdbcreate $db

eyedbodl -d $db -u schema.odl
