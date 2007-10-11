#!/bin/bash

function ssh_upload() {
    WEB_HOST="$1"
    WEB_DIR="$2"
    shift 2
    ssh $WEB_HOST mkdir $WEB_DIR
    scp $* $WEB_HOST:$WEB_DIR
}

function ftp_upload() {
    WEB_HOST="$1"
    WEB_DIR="$2"
    shift 2
    FILES="$*"
    ncftp $WEB_HOST <<EOF
set confirm-close no
cd $WEB_DIR
mput $FILES
quit
EOF
}

UPLOAD_METHOD="$1"
shift 1

case "$UPLOAD_METHOD" in
--ssh)
	ssh_upload $*
;;
--ftp)
	ftp_upload $*
;;
esac
