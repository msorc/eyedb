noinst_PROGRAMS= quicktour

nodist_quicktour_SOURCES= quicktour.cc quicktour-benchmark.cc

EYEDB_INCLUDEDIR=@EYEDB_INCLUDEDIR@
INCLUDES=-I$(EYEDB_INCLUDEDIR) -I$(top_srcdir)/src/cpp
EYEDB_LIBDIR=@EYEDB_LIBDIR@
LDADD=-L$(EYEDB_LIBDIR) -leyedb -leyedbrpcfe -leyedbsm -leyedbutils -lpthread $(top_builddir)/src/cpp/framework/libeyedbbenchmark.la

EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDBODLCXX= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLCXXFLAGS= --gencode=C++ --package=quicktour

quicktour.cc quicktour.h : quicktour.odl
	$(EYEDBODLCXX) $(EYEDBODLCXXFLAGS) $<

CLEANFILES= quicktour.cc quicktour.h Makefile.quicktour quicktourmthbe-skel.cc quicktourmthfe-skel.cc quicktourstubsbe.cc quicktourstubsfe.cc template_quicktour.cc
