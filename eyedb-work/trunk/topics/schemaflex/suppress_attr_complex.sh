
db=foo1
dbdelete $db
dbcreate $db
odlfile=suppress_attr_complex.odl

set -e

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db
odl -d $db --cpp-flags="-DUPDATE1" -u $odlfile
oql -c "select schema" -p -d $db

#oql -c "select schema" -p -d $db
#gdb --args `type -p oql` -d $db #-c "select schema" -p
