#!/bin/bash
set -x
aclocal -I m4/java -I m4/swig -I m4/python
libtoolize --force --automake --copy
autoheader
automake  --foreign --add-missing --copy
autoconf 
