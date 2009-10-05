#!/bin/bash
DATABASE=test_collection

function do_version()
{
    VERSION=$1
    TEST=test_collection_$VERSION
    DATABASE=$TEST

    echo "## delete database $DATABASE"
    eyedbadmin database delete $DATABASE
    echo

    echo "## create database $DATABASE"
    eyedbadmin database create $DATABASE
    eyedbodl -u -d $DATABASE $TEST.odl
    echo

    echo "## getting and setting default implementation"
    eyedbadmin collection getdefimpl $DATABASE Diary.persons
    eyedbadmin collection setdefimpl --type=hashindex $DATABASE Diary.persons
    eyedbadmin collection getdefimpl $DATABASE Diary.persons
    echo

    echo "### inserting some values"
    eyedboql -w --echo --commit -d $DATABASE < $TEST.oql

    echo "## getting and updating a collection implementation"
    COLLECTION='select one Diary.persons':oql
    eyedbadmin collection getimpl $DATABASE "$COLLECTION"
    eyedbadmin collection updateimpl --type=btreeindex $DATABASE "$COLLECTION"
    echo

    echo "## getting statistics on a collection implementation"
    eyedbadmin collection statsimpl $DATABASE "$COLLECTION"
    echo
}

do_version 1
do_version 2

exit 0




echo "## Getting and setting collection default dataspace"
echo "# get default dataspace for collection ???"
eyedbadmin collection getdefdsp
echo "# create two datafiles"
eyedbadmin datafile create --name=DAT1 $DATABASE ${DATABASE}_data1.dat
eyedbadmin datafile create --name=DAT2 $DATABASE ${DATABASE}_data2.dat
echo "# create a dataspace containing these files"
eyedbadmin dataspace create $DATABASE DSP1 DAT1 DAT2
echo "# set default dataspace for collection"
eyedbadmin collection setdefdsp $DATABASE "$COLLECTION" DSP1
echo "# get default dataspace for collection"
eyedbadmin collection getdefdsp $DATABASE "$COLLECTION" 
echo
