HIBERNATE_JAR=@HIBERNATE_JAR@
HIBERNATE_CLASSPATH=@HIBERNATE_CLASSPATH@
JDBC_CONNECTOR_JAR=@JDBC_CONNECTOR_JAR@

packagedir=$(pkglibdir)/java/classes/quicktour/hibernate

dist_package_JAVA =\
Benchmark.java \
CreateBenchmark.java \
Person.java \
Student.java \
Course.java \
Teacher.java

JAVAROOT=$(top_builddir)/src/java/quicktour/hibernate
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmarks.jar:$(top_srcdir)/src/java:$(HIBERNATE_JAR)

noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@HIBERNATE_JAR\@,$(HIBERNATE_JAR),g' \
	-e 's,@HIBERNATE_CLASSPATH\@,$(HIBERNATE_CLASSPATH),g '\
	-e 's,@JDBC_CONNECTOR_JAR\@,$(JDBC_CONNECTOR_JAR),g '\
        -e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

EXTRA_DIST= hibernate.cfg.xml Course.hbm.xml Person.hbm.xml Student.hbm.xml Teacher.hbm.xml
