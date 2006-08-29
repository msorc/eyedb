#!/bin/bash
set -x
[ ! -d config/ ] && mkdir config/
aclocal -I m4/extras/ -I m4/java/
# On Mac OS X, libtoolize is named glibtoolize
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
automake  --foreign --add-missing --force-missing --copy
autoconf 
