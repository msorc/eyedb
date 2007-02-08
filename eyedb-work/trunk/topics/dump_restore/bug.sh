
db=dump_restore

dbdelete $db
dbcreate $db
odl -d $db -u schema.odl

(sleep 5; eyedbdbexport $db $db.DAT) &

echo type:
echo "select C;"
echo "\p"
oql -d $db -w


