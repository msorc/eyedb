#!/bin/sh
#
# init.sh

if [ $# != 1 ]
then
    echo "usage: $0 DBNAME"
    exit 1
fi

eyedb_dbdelete $1
set -e
eyedb_dbcreate $1
eyedbodl -d $1 -u schema.odl

