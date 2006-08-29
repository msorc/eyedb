#!/bin/bash
set -x
[ ! -d config/ ] && echo mkdir config/
aclocal
# On Mac OS X, libtoolize is named glibtoolize
libtoolize --force --automake --copy || glibtoolize --force --automake --copy
automake  --foreign --add-missing --force-missing --copy
autoconf 
