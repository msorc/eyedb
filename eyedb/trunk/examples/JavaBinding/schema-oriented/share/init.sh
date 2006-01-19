#!/bin/sh
#
# init.sh

set -e

db=person_j

echo Creating the database $db: \'eyedbdbcreate $db\'...

eyedbdbcreate $db

echo
echo Changing the default access to read/write: \'eyedbdbaccess $db rw\'...
eyedbdbaccess $db rw

echo
echo Updating the database $db with the schema person...

eyedbodl -update -db $db -package $db person.odl

echo Creating a few instances in the database $db ...

eyedboql -db $db -rw << EOF
!settermchar ;

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

!commit

!quit

EOF
