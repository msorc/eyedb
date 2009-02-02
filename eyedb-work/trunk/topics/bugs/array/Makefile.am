BUILT_SOURCES= array_odl.cc array_odl.h coll_odl.cc coll_odl.h
ODL_GENERATED_FILES= $(P_).cc $(P_).h $(P_)mthbe-skel.cc $(P_)mthfe-skel.cc $(P_)stubsbe.cc $(P_)stubsfe.cc template_$(P_).cc Makefile.$(P_)
ODL_PACKAGES=array_odl coll_odl
CLEANFILES=$(BUILT_SOURCES) $(foreach P_, $(ODL_PACKAGES), $(ODL_GENERATED_FILES))

#
# Programs
#
noinst_PROGRAMS= testarray testcoll
INCLUDES=$(EYEDB_CFLAGS)
LDADD=$(EYEDB_LIBS)

testarray_SOURCES= testarray.cc array_odl.cc
testcoll_SOURCES= testcoll.cc coll_odl.cc

# 
# ODL -> C++
# 
EYEDBODLCXX = $(EYEDB_BINDIR)/eyedbodl --gencode=C++
EYEDBODLCXX_FLAGS= 

%_odl.cc %_odl.h : %.odl
	$(EYEDBODLCXX) $(EYEDBODLCXX_FLAGS) --package=$*_odl $<

#
# Scripts
#
noinst_DATA=runit_array.sh runit_coll.sh
EDIT = sed \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_LIBDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_srcdir[@],$(top_srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g'

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh



