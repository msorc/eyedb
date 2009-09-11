#!/bin/bash
bash -x $1 2>&1 | sed  -e '/^\+ /s/$/<\/userinput>/' -e 's/^\+ /<userinput>/' -e '/<userinput>echo/d' -e '/<userinput>tail /d'
