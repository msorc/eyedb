#!/bin/bash
DATABASE=test_collection

echo "## delete database"
eyedbadmin database delete $DATABASE
echo

echo "## create database"
eyedbadmin database create $DATABASE
eyedbodl -u -d $DATABASE test_collection.odl
echo

echo "## getting and setting default implementation"
eyedbadmin collection getdefimpl $DATABASE Diary.persons
eyedbadmin collection setdefimpl --type=hashindex $DATABASE Diary.persons
eyedbadmin collection getdefimpl $DATABASE Diary.persons
echo

exit 0
echo "## getting and updating a collection implementation"
eyedbadmin collection getimpl
eyedbadmin collection updateimpl
echo

echo "## getting statistics on a collection implementation"
eyedbadmin collection statsimpl
echo

echo "## Getting and setting collection default dataspace"
echo "# get default dataspace for collection ???"
eyedbadmin collection getdefdsp
echo "# create two datafiles"
eyedbadmin datafile create --name=DAT1 $DATABASE ${DATABASE}_data1.dat
eyedbadmin datafile create --name=DAT2 $DATABASE ${DATABASE}_data2.dat
echo "# create a dataspace containing these files"
eyedbadmin dataspace create $DATABASE DSP1 DAT1 DAT2
echo "# set default dataspace for collection ???"
eyedbadmin collection setdefdsp
echo "# get default dataspace for collection ???"
eyedbadmin collection getdefdsp
echo
