DB4O_JAR=@DB4O_JAR@

packagedir=$(pkglibdir)/java/classes/quicktour/db4o

dist_package_JAVA =\
Benchmark.java \
CreateBenchmark.java \
Person.java \
Student.java \
Course.java \
Teacher.java

JAVAROOT=$(top_builddir)/src/java/quicktour/db4o
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(DB4O_JAR)

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
