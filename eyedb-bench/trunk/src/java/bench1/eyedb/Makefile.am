EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDB_SBINDIR=@EYEDB_SBINDIR@
EYEDB_JAR=@EYEDB_JAR@

EYEDBODLJAVA = $(EYEDB_BINDIR)/eyedbodl --gencode=java
EYEDBODLJAVAFLAGS= --no-rootclass --package=person

packagedir=$(pkglibdir)/java/classes/bench1/eyedb/

JAVA_ODL_FILES=$(addprefix person/, Database.java Person.java)

dist_package_JAVA =\
EyedbBench.java \
$(JAVA_ODL_FILES)

JAVAROOT=$(top_builddir)/src/java/bench1/eyedb
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmarks.jar:$(top_srcdir)/src/java:$(EYEDB_JAR)

TESTS_ENVIRONMENT=$(SHELL)
TESTS=runit.sh

noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@EYEDB_BINDIR[@],$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR[@],$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_JAR[@],$(EYEDB_JAR),g' \
	-e 's,@JAVA[@],$(JAVA),g' 

$(JAVA_ODL_FILES): person.odl
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

clean-local:
	-rm -f $(JAVA_ODL_FILES)
	-rm -f person/*.class

