DATABASE=test_index

bash -x <<EOF 2>&1 | sed  -e '/^\+ /s/$/<\/userinput>/' -e 's/^\+ /<userinput>/'
echo "... create database"
eyedbadmin database create $DATABASE
eyedbodl -u -d $DATABASE test_index.odl

echo "... create index"
eyedbadmin index create $DATABASE Person.age
eyedbadmin index create $DATABASE Person.addr.town
eyedbadmin index create $DATABASE Person.addr.number
eyedbadmin index create --type=hash $DATABASE Person.addr.code

echo "... delete index"
eyedbadmin index delete $DATABASE Person.addr.number
eyedbadmin index delete $DATABASE Person.addr.street

echo "... list index"
eyedbadmin index list $DATABASE
eyedbadmin index list $DATABASE Person
eyedbadmin index list $DATABASE Person.age

echo "... stats index"
eyedbadmin index stats --full $DATABASE | tail -6
eyedbadmin index stats --format="%n %O\n" $DATABASE Person.addr.code | tail -6
eyedbadmin index stats --format="%n -> %O, %o, %s\n" $DATABASE Person.addr.town | tail -6
EOF
 
