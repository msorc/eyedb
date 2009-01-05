#!/bin/bash
#
# bugrmsch.sh
#

PREFIX=$(dirname $(which eyedboql))/..

db=bug_rmsch

eyedb_dbdelete $db

eyedb_dbcreate $db

eyedbodl -d $db -u $PREFIX/share/doc/eyedb/examples/GettingStarted/schema.odl

# this:
eyedbadmin index delete bug_rmsch Person.firstname

# or this:
#eyedbodl --rmsch -d $db -u
# produces server failure: the EyeDB server has probably crashed or timed out.
