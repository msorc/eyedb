#!/bin/bash
DATABASE=test_index

echo "## delete database"
eyedbadmin database delete $DATABASE
echo

echo "## create database"
eyedbadmin database create $DATABASE
eyedbodl -u -d $DATABASE test_index.odl
echo

echo "## create index"
eyedbadmin index create $DATABASE Person.age
eyedbadmin index create $DATABASE Person.addr.town
eyedbadmin index create $DATABASE Person.addr.number
eyedbadmin index create --type=hash $DATABASE Person.addr.code
echo

echo "## delete index"
eyedbadmin index delete $DATABASE Person.addr.number
eyedbadmin index delete $DATABASE Person.addr.street
echo

echo "## list index"
eyedbadmin index list $DATABASE
eyedbadmin index list $DATABASE Person
eyedbadmin index list $DATABASE Person.age
echo

echo "## stats index"
eyedbadmin index stats --full $DATABASE | tail -6
eyedbadmin index stats --format="%n %O\n" $DATABASE Person.addr.code | tail -6
eyedbadmin index stats --format="%n -> %O, %o, %s\n" $DATABASE Person.addr.town | tail -6
echo

echo "## get/setdefdsp  index"
echo "# get default dataspace for index on Person.age"
eyedbadmin index getdefdsp $DATABASE Person.age
echo "# create a datafile"
eyedbadmin datafile create --name=DAT1 $DATABASE ${DATABASE}_data1.dat
echo "# create a dataspace containing this file"
eyedbadmin dataspace create $DATABASE DSP1 DAT1
echo "# set default dataspace for index on Person.age"
eyedbadmin index setdefdsp $DATABASE Person.age DSP1
echo "# get default dataspace for index on Person.age"
eyedbadmin index getdefdsp $DATABASE Person.age
echo
