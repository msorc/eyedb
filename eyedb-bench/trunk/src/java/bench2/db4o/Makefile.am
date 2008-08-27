DB4O_JAR=@DB4O_JAR@

packagedir=$(pkglibdir)/java/classes/bench2/db4o

dist_package_JAVA =\
Db4oServer.java \
Db4oBench1.java \
Person.java

JAVAROOT=$(top_builddir)/src/java/bench2/db4o
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmarks.jar:$(top_srcdir)/src/java:$(DB4O_JAR)

noinst_DATA=runit.sh

EDIT = sed \
        -e 's,@DB4O_JAR\@,$(DB4O_JAR),g' \
        -e 's,@JAVA\@,$(JAVA),g' 

runit.sh: Makefile $(srcdir)/runit.sh.in
	rm -f $@ $@.tmp
	$(EDIT) $(srcdir)/$@.in >$@.tmp
	mv $@.tmp $@
