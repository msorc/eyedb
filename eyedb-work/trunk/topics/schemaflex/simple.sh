
db=foo1
dbdelete $db
dbcreate $db

odlfile=simple.odl
set -e

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db
gdb --args `type -p odl` -d $db --cpp-flags="-DUPDATE0" -u $odlfile

#odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile

