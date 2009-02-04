bin_PROGRAMS= quicktour-array quicktour-noinverse-set quicktour-noinverse-bag quicktour-inverse

ODL_PACKAGES=\
odl_quicktour_array \
odl_quicktour_noinverse_set \
odl_quicktour_noinverse_bag \
odl_quicktour_inverse

nodist_quicktour_array_SOURCES= \
quicktour_benchmark.cc \
quicktour_benchmark_array.cc \
odl_quicktour_array.cc
nodist_quicktour_noinverse_set_SOURCES= \
quicktour_benchmark.cc \
quicktour_benchmark_noinverse_set.cc \
odl_quicktour_noinverse_set.cc
nodist_quicktour_noinverse_bag_SOURCES= \
quicktour_benchmark.cc \
quicktour_benchmark_noinverse_bag.cc \
odl_quicktour_noinverse_bag.cc
nodist_quicktour_inverse_SOURCES= \
quicktour_benchmark.cc \
quicktour_benchmark_inverse.cc \
odl_quicktour_inverse.cc

# tracing the eyedbsm message
AM_CPPFLAGS=-DLOOKING_FOR_BUG

EYEDB_INCLUDEDIR=@EYEDB_INCLUDEDIR@
INCLUDES=-I$(EYEDB_INCLUDEDIR) -I$(top_srcdir)/src/cpp
EYEDB_LIBDIR=@EYEDB_LIBDIR@
LDADD=-L$(EYEDB_LIBDIR) -leyedb -leyedbrpcfe -leyedbsm -leyedbutils -lpthread $(top_builddir)/src/cpp/framework/libeyedbbenchmark.a

EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDBODL= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLCXXFLAGS= --gencode=C++ 

BUILT_SOURCES= \
odl_quicktour_array.cc \
odl_quicktour_array.h \
odl_quicktour_noinverse_set.cc \
odl_quicktour_noinverse_set.h \
odl_quicktour_noinverse_bag.cc \
odl_quicktour_noinverse_bag.h \
odl_quicktour_inverse.cc \
odl_quicktour_inverse.h 

odl_quicktour_array.cc odl_quicktour_array.h : quicktour.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_quicktour_array --cpp-flags=-DUSE_ARRAY $<

odl_quicktour_noinverse_bag.cc odl_quicktour_noinverse_bag.h : quicktour.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_quicktour_noinverse_bag --cpp-flags="-UUSE_ARRAY -UUSE_INVERSE -DCOLLECTION_TYPE=bag" $<

odl_quicktour_noinverse_set.cc odl_quicktour_noinverse_set.h : quicktour.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_quicktour_noinverse_set --cpp-flags="-UUSE_ARRAY -UUSE_INVERSE -DCOLLECTION_TYPE=set" $<

odl_quicktour_inverse.cc odl_quicktour_inverse.h : quicktour.odl
	$(EYEDBODL) $(EYEDBODLCXXFLAGS) --package=odl_quicktour_inverse --cpp-flags=-DUSE_INVERSE $<

ODL_GENERATED_FILES= $(P_).cc $(P_).h $(P_)mthbe-skel.cc $(P_)mthfe-skel.cc $(P_)stubsbe.cc $(P_)stubsfe.cc template_$(P_).cc Makefile.$(P_)
CLEANFILES= $(foreach P_, $(ODL_PACKAGES), $(ODL_GENERATED_FILES))

