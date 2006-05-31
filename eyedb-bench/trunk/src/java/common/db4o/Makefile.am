DB4O_JAR=@DB4O_JAR@

packagedir=$(pkglibdir)/java/classes/common/db4o

dist_package_JAVA =\
Db4oServer.java 

JAVAROOT=$(top_builddir)/src/java/common/db4o
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(DB4O_JAR)

noinst_DATA=run-server.sh

EDIT = sed \
        -e 's,@DB4O_JAR\@,$(DB4O_JAR),g' \
        -e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

run-server.sh: Makefile
