
db=foo1
dbdelete $db
dbcreate $db

odlfile=rename_attr.odl
set -e

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db

EYEDBWAIT=1 gdb --args odl -d $db --cpp-flags="-DUPDATE1" -u $odlfile
oql -c "select schema" -p -d $db

EYEDBWAIT=1 gdb --args odl -d $db --cpp-flags="-DUPDATE2" -u $odlfile
oql -c "select schema" -p -d $db
