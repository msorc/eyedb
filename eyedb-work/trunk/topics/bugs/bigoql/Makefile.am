noinst_PROGRAMS= insert_packet

ODL_PACKAGES=\
odl_sample

nodist_insert_packet_SOURCES= \
insert_packet.cc \
odl_sample.cc

INCLUDES=$(EYEDB_CFLAGS)
LDADD=$(EYEDB_LIBS)

EYEDBODL= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLCXXFLAGS= --gencode=C++ 

BUILT_SOURCES= \
odl_sample.cc \
odl_sample.h 

odl_sample.cc odl_sample.h : sample.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_sample --cpp-flags=-DUSE_INVERSE $<

ODL_GENERATED_FILES= $(P_).cc $(P_).h $(P_)mthbe-skel.cc $(P_)mthfe-skel.cc $(P_)stubsbe.cc $(P_)stubsfe.cc template_$(P_).cc Makefile.$(P_)
CLEANFILES= $(foreach P_, $(ODL_PACKAGES), $(ODL_GENERATED_FILES)) $(noinst_DATA)

noinst_DATA=runit.sh
EDIT = sed \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_LIBDIR\@,$(EYEDB_BINDIR),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

run: runit.sh
	$(SHELL) -x runit.sh
