DB4O_JAR=@DB4O_JAR@

packagedir=$(pkglibdir)/java/classes/bench1/db4o

dist_package_JAVA =\
Db4oServer.java \
Db4oBench.java \
Db4oBenchLocal.java \
Person.java

JAVAROOT=$(top_builddir)/src/java/bench1/db4o
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmarks.jar:$(top_srcdir)/src/java:$(DB4O_JAR)

noinst_DATA=runit.sh runit-local.sh

EDIT = sed \
        -e 's,@DB4O_JAR\@,$(DB4O_JAR),g' \
        -e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile
runit-local.sh: Makefile
