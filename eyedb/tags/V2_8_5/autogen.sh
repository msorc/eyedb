#!/bin/sh
set -x
aclocal -I m4/java -I m4/swig -I m4/python -I m4/extras
# On Mac OS X, libtoolize is named glibtoolize
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
autoheader
automake  --foreign --add-missing --copy
autoconf 
