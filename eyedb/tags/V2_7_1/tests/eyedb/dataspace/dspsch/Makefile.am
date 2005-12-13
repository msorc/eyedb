packagedir=$(pkglibdir)/tests/eyedb/dataspace
dist_package_JAVA= \
Database.java \
O0.java \
O1_1_1.java \
O1_1.java \
O1.java \
O2.java \
O3.java \
O4.java \
O5.java

JAVAROOT=$(top_builddir)/tests/eyedb/dataspace
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/tests/eyedb/dataspace:$(top_builddir)/src/java/eyedb.jar

#
# ODL to Java translation
#
EYEDBODL_exe=$(bindir)/eyedbodl

EYEDBODL = $(EYEDBODL_exe) -output-dir $(top_srcdir)/tests/eyedb/dataspace/dspsch
EYEDBODLFLAGS = -gencode Java -no-rootclass

#$(top_srcdir)/tests/eyedb/dataspace/dspsch/Database.java: $(top_srcdir)/tests/eyedb/dataspace/dspsch.odl

$(top_srcdir)/tests/eyedb/dataspace/dspsch/Database.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O0.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O1_1_1.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O1_1.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O1.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O2.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O3.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O4.java $(top_srcdir)/tests/eyedb/dataspace/dspsch/O5.java : $(top_srcdir)/tests/eyedb/dataspace/dspsch.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found, using $*.java old Java code; touch $*.java ; \
	fi

