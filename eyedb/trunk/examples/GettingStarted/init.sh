#!/bin/sh
set -e
SCHEMA=$1
DATABASE=person_g

echo Creating the database $DATABASE
echo eyedbdbcreate $DATABASE
eyedbdbcreate $DATABASE

echo
echo Changing the default access to read/write
echo eyedbdbaccess $DATABASE rw
eyedbdbaccess $DATABASE rw

echo
echo Updating the database $DATABASE with the schema person...
echo eyedbodl --update --database=$DATABASE $SCHEMA
eyedbodl --update --database=$DATABASE $SCHEMA

echo Creating a few instances in the database $DATABASE ...
echo eyedboql -d $DATABASE -w
eyedboql -d $DATABASE -w << EOF
john := new Person( firstname : "john", lastname : "wayne", age : 72);
mary := new Person( firstname : "mary", lastname : "poppins", age : 68);
john.spouse := mary;
add Person(firstname : "baby1", age : 2) to john->children;
add Person(firstname : "baby2", age : 3) to john->children;
add Person(firstname : "baby3", age : 4) to john->children;
\commit
\quit
EOF
