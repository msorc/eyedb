#!/bin/bash
EYEDB_BINDIR=/home/francois/projects/eyedb/install/bin
EYEDB_LIBDIR=/home/francois/projects/eyedb/install/lib
EYEDB_JAR=/home/francois/projects/eyedb/install/lib/eyedb/java/eyedb.jar
JAVA=/usr/local/java/jdk1.5.0_05/bin/java

PROPERTIES=eyedb.properties
DATABASE=`grep database $PROPERTIES | awk -F = '{print $2}'`
PORT=`grep tcp_port $PROPERTIES | awk -F = '{print $2}'`
SCHEMA=person.odl

start_server() {
    $EYEDB_BINDIR/eyedbrc start --listen=$PORT --nod
}

delete_db() {
    if $EYEDB_BINDIR/eyedbdblist --port=$PORT $DATABASE > /dev/null 2>&1; then
	$EYEDB_BINDIR/eyedbdbdelete --port=$PORT $DATABASE
    fi
}

create_db() {
    if $EYEDB_BINDIR/eyedbdblist --port=$PORT $DATABASE > /dev/null 2>&1; then
	:
    else
	$EYEDB_BINDIR/eyedbdbcreate --port=$PORT $DATABASE
	$EYEDB_BINDIR/eyedbdbaccess --port=$PORT $DATABASE rw
	$EYEDB_BINDIR/eyedbodl --port=$PORT --update --database=$DATABASE $SCHEMA
    fi
}

run_benchmark() {
    CLASSPATH=.:../../eyedb-benchmarks.jar:$EYEDB_JAR
    CLASSPATH=$CLASSPATH $JAVA org.eyedb.benchmark.Run EyedbBench1 $PROPERTIES
}

stop_server() {
    $EYEDB_BINDIR/eyedbrc stop --listen=$PORT 
}

stop_server
set -e
start_server
delete_db
create_db
run_benchmark
stop_server

