
db=foo1
dbdelete $db
dbcreate $db

odlfile=rename_cls_attr.odl
set -e

odl -d $db --cpp-flags="-DUPDATE0" -u $odlfile
oql -c "select schema" -p -d $db

odl -d $db --cpp-flags="-DUPDATE1" -u $odlfile
oql -c "select schema" -p -d $db
