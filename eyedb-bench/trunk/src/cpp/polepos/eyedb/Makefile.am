noinst_PROGRAMS= barcelona bahrain imola melbourne sepang

nodist_barcelona_SOURCES= polepos.cc polepos-benchmark.cc barcelona.cc

nodist_bahrain_SOURCES= polepos.cc polepos-benchmark.cc bahrain.cc

nodist_imola_SOURCES= polepos.cc polepos-benchmark.cc imola.cc

nodist_melbourne_SOURCES= polepos.cc polepos-benchmark.cc melbourne.cc

nodist_sepang_SOURCES= polepos.cc polepos-benchmark.cc treeutil.cc sepang.cc

EYEDB_INCLUDEDIR=@EYEDB_INCLUDEDIR@
INCLUDES=-I$(EYEDB_INCLUDEDIR) -I$(top_srcdir)/src/cpp
EYEDB_LIBDIR=@EYEDB_LIBDIR@
LDADD=-L$(EYEDB_LIBDIR) -leyedb -leyedbrpcfe -leyedbsm -leyedbutils -lpthread $(top_builddir)/src/cpp/framework/libeyedbbenchmark.la

EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDBODLCXX= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLCXXFLAGS= --gencode=C++ --package=polepos

polepos.cc polepos.h : polepos.odl
	$(EYEDBODLCXX) $(EYEDBODLCXXFLAGS) $<

CLEANFILES= polepos.cc polepos.h Makefile.polepos poleposmthbe-skel.cc poleposmthfe-skel.cc poleposstubsbe.cc poleposstubsfe.cc template_polepos.cc
