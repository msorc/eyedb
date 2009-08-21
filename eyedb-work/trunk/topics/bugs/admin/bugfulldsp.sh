#!/bin/bash
#
# bugfulldsp.sh
#

list_datafiles()
{
    echo "Datafile list for Dataspace $1"
    eyedbadmin dataspace list $db $1 | grep "Name   " | sed -e 's/Name//'
}

perform()
{
    echo
    echo Executing: $*
    eval $*
}

db=foo4

eyedb_dbdelete $db

eyedb_dbcreate $db

for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
do
  eyedbadmin datafile create --size=10 $db ${db}${i}.dat --name=alpha${i}
done

perform eyedbadmin dataspace create $db DSP_1
list_datafiles DSP_1

perform eyedbadmin dataspace create $db DSP_2 alpha01 alpha02
list_datafiles DSP_2

perform eyedbadmin dataspace create $db DSP_3 alpha03 alpha04
list_datafiles DSP_3

perform eyedbadmin dataspace update $db DSP_1 alpha05 alpha06
list_datafiles DSP_1

perform eyedbadmin dataspace add    $db DSP_2 alpha07 alpha08
list_datafiles DSP_2

perform eyedbadmin dataspace add    $db DSP_3 alpha09 alpha10
list_datafiles DSP_3


perform eyedbadmin dataspace update $db DSP_1 alpha11 --migrate-orphan=DSP_1

list_datafiles DSP_1
list_datafiles DSP_3


perform eyedbadmin dataspace update $db DSP_1 alpha11 --migrate-orphan=DSP_3

list_datafiles DSP_1
list_datafiles DSP_3

perform eyedbadmin dataspace add    $db DSP_3 alpha09 alpha10
list_datafiles DSP_3
perform eyedbadmin dataspace add    $db DSP_3 alpha09 alpha10
list_datafiles DSP_3
perform eyedbadmin dataspace add    $db DSP_3 alpha09 alpha10
list_datafiles DSP_3

perform eyedbadmin dataspace create $db DSP_4 alpha12 alpha13 alpha13 alpha13 alpha12
list_datafiles DSP_4



