#!/bin/sh
set -e
SCHEMA=$1
DATABASE=person_j

echo Creating the database $DATABASE
echo eyedbadmin database create $DATABASE
eyedbadmin database create $DATABASE

echo
echo Changing the default access to read/write
echo eyedbadmin database defaccess $DATABASE rw
eyedbadmin database defaccess $DATABASE rw

echo
echo Updating the database $DATABASE with the schema person...
echo eyedbodl --update --database=$DATABASE $SCHEMA
eyedbodl --update --database=$DATABASE --package=$DATABASE $SCHEMA

echo
echo Creating a few instances in the database $DATABASE ...
echo eyedboql -d $DATABASE -w
eyedboql -d $DATABASE -w << EOF
john := new Person(
    name        : "john", 
    age         : 32,
    addr.street : "clichy",
    addr.town   : "Paris",
    cstate      : Sir);
mary := new Person(
    name        : "mary", 
    age         : 30,
    addr.street : "clichy",
    addr.town   : "Paris",
    spouse      : john,
    cstate      : Lady);
for (x in interval(1, 4))
 add (new Car(mark : "renault", num : x+100)) to john->cars;
henry := new Employee(
   name        : "henry",
   salary      : 100000,
   age         : 32,
   addr.street : "parmentier",
   addr.town   : "Paris",
   cstate      : Sir);
nadou := new Employee(
   name        : "nadou",
   salary      : 20000,
   age         : 30,
   addr.street : "parmentier",
   addr.town   : "Paris",
   spouse      : henry,
   cstate      : Sir);
for (x in interval(1, 8))
 add (new Car(mark : "renault", num : x+100)) to nadou->cars;
\commit
\quit
EOF
