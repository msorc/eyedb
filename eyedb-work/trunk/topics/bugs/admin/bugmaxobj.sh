#!/bin/bash
#
# bugmaxobj.sh
#

db=foo4

eyedb_dbdelete $db
eyedb_dbcreate $db --max-object-count=100000000000

# gives a bad result for Maxx Object Count
eyedb_dblist $db

# fixed 2009-03-19 by EV
