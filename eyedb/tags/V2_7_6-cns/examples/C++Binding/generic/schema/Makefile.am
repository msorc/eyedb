eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)/examples/C++Binding/generic/schema
dist_eyedbexamples_DATA=object_dump.cc schema_dump.cc
#eyedbexamples_DATA=Makefile.eyedb-examples

check_PROGRAMS= schema_dump object_dump

schema_dump_SOURCES= schema_dump.cc
schema_dump_LDADD= $(EYEDBSVLIBS)
object_dump_SOURCES= object_dump.cc
object_dump_LDADD= $(EYEDBSVLIBS)

EYEDBSVLIBS=$(top_builddir)/src/eyedb/libeyedb.la $(top_builddir)/src/lib/libeyedbrpcbe.la $(top_builddir)/src/lib/libeyedbrpcfe.la $(top_builddir)/src/eyedbsm/libeyedbsm.la $(top_builddir)/src/lib/libeyedbutils.la 

# The last directory is needed for endian.h, which is generated by configure.in
INCLUDES=-I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_builddir)/include
