BUILT_SOURCES = eyedbreflect_wrap.cpp
SWIG_SOURCES = $(srcdir)/eyedbpython.i

nodist_pkgpython_PYTHON = eyedbreflect.py
pkgpyexec_LTLIBRARIES= eyedbreflect.la
nodist_eyedbreflect_la_SOURCES= eyedbreflect_wrap.cpp $(SWIG_SOURCES)
eyedbreflect_la_CPPFLAGS= $(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/src
eyedbreflect_la_LDFLAGS= -module
#_eyedbreflect_la_LIBADD = ../src/libproject.la
# The last directory is needed for endian.h, which is generated by configure.in
INCLUDES=-I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_builddir)/include -I$(top_srcdir)/src/bindings/common/interfaces -I$(top_srcdir)/src/bindings/common/src

SWIG_INCLUDES=-I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/src/bindings/common/interfaces -I$(top_srcdir)/src/bindings/common/src
SWIG_EXTRA_FLAGS=-w362 -w389 -w509 -w312 -w401 -w503 -module eyedbreflect

SED_COMMANDS= \
-e 's/delete arg1/delete_or_release(arg1)/' \
-e 's/result = .*arg1)->get.*OidAt(arg2,arg3)/result = idbOid()/' \
-e 's/result = .*arg1)->get.*OidAt(arg2)/result = idbOid()/' \
-e 's/result = .*arg1)->retrieve.*OidAt(arg2,arg3)/result = idbOid()/' \
-e 's/result = .*arg1)->retrieve.*OidAt(arg2)/result = idbOid()/' \
-e 's/result = (arg1)->unset.*CollAt((idbOid const &)\*arg2)/result = 0/' 

eyedbreflect_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) $(SWIG_INCLUDES) $(SWIG_EXTRA_FLAGS) -o swig.tmp $< 
	sed $(SED_COMMANDS) swig.tmp > $@
	rm swig.tmp

eyedbreflect_wrap.cpp : $(top_srcdir)/src/bindings/common/interfaces/eyedbreflect.i $(top_srcdir)/src/bindings/common/interfaces/eyedbschema.i

clean-local:
	rm -f $(BUILT_SOURCES) $(pkgpython_PYTHON)

#SWIGFLAGS=-python -c++ -w362 -w389 -w509 -w312 -w401 -w503 -module eyedbreflect $(SWIGINCLUDES)
#swig -python -c++ -w362 -w389 -w509 -w312 -w401 -w503 -module eyedbreflect -o swout.tmp -DLINUX -I/home/francois/projects/eyedb/install/include -I/home/francois/projects/eyedb/src/eyedb-cvs/src/bindings_org/common/interfaces -I/home/francois/projects/eyedb/src/eyedb-cvs/src/bindings_org/common/src tmp.i
