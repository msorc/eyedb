
db=foo1
dbdelete $db
dbcreate $db

odlfile=suppress_attr.odl

set -e

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db
odl -d $db --diff --cpp-flags="-DUPDATE1" $odlfile
#gdb --args `type -p odl` -d $db --diff --cpp-flags="-DUPDATE1" $odlfile

odl -d $db --cpp-flags="-DUPDATE1" -u $odlfile
oql -c "select schema" -p -d $db

exit 0

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db
odl -d $db --cpp-flags="-DUPDATE1" -u $odlfile
oql -c "select schema" -p -d $db
odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db
