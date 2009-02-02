noinst_PROGRAMS= quicktour-inverse

ODL_PACKAGES=\
odl_quicktour_inverse

nodist_quicktour_inverse_SOURCES= \
quicktour_benchmark.cc \
quicktour_benchmark_inverse.cc \
odl_quicktour_inverse.cc \
properties.cc \
benchmark.cc 

INCLUDES=$(EYEDB_CFLAGS)
LDADD=$(EYEDB_LIBS)

EYEDBODL= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLCXXFLAGS= --gencode=C++ 

BUILT_SOURCES= \
odl_quicktour_inverse.cc \
odl_quicktour_inverse.h 

odl_quicktour_inverse.cc odl_quicktour_inverse.h : quicktour.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_quicktour_inverse --cpp-flags=-DUSE_INVERSE $<

ODL_GENERATED_FILES= $(P_).cc $(P_).h $(P_)mthbe-skel.cc $(P_)mthfe-skel.cc $(P_)stubsbe.cc $(P_)stubsfe.cc template_$(P_).cc Makefile.$(P_)
CLEANFILES= $(foreach P_, $(ODL_PACKAGES), $(ODL_GENERATED_FILES)) $(noinst_DATA)

noinst_DATA=runit.sh
EDIT = sed \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_LIBDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_srcdir[@],$(top_srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

