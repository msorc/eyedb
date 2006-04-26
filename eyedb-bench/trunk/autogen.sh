#!/bin/bash
set -x
aclocal -I m4/java
# On Mac OS X, libtoolize is named glibtoolize
#libtoolize --force --automake --copy || glibtoolize --force --automake --copy
#autoheader
automake  --foreign --add-missing --copy
autoconf 
