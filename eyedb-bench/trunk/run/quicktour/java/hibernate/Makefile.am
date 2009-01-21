HIBERNATE_JAR=@HIBERNATE_JAR@
HIBERNATE_CLASSPATH=@HIBERNATE_CLASSPATH@
JDBC_CONNECTOR_JAR=@JDBC_CONNECTOR_JAR@
SLF4J_JAR=@SLF4J_JAR@

noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' \
	-e 's,@HIBERNATE_JAR[@],$(HIBERNATE_JAR),g' \
	-e 's,@HIBERNATE_CLASSPATH[@],$(HIBERNATE_CLASSPATH),g '\
	-e 's,@JDBC_CONNECTOR_JAR[@],$(JDBC_CONNECTOR_JAR),g '\
	-e 's,@SLF4J_JAR[@],$(SLF4J_JAR),g '\
        -e 's,@JAVA[@],$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

bench: runit.sh
	$(SHELL) runit.sh

EXTRA_DIST=hibernate.properties
