packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/framework/reporter/openoffice

dist_package_JAVA =\
OpenOfficeReporter.java \
OpenOfficeReporterTest.java 

JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(OPENOFFICE_CLASSPATH)

EDIT = sed \
	-e 's,@top_builddir[@],$(top_builddir),g' \
	-e 's,@JAVA\@,$(JAVA),g' \
	-e 's,@JAVAROOT\@,$(JAVAROOT),g' \
	-e 's,@OPENOFFICE_CLASSPATH\@,$(OPENOFFICE_CLASSPATH),g'

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

TESTS_ENVIRONMENT= bash -x
TESTS= runtest.sh

