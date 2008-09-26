packagedir= $(pkglibdir)/java/classes/org/eyedb/benchmark/polepos/teams/eyedb/data
dist_package_JAVA= $(JAVA_ODL_FILES)
JAVAROOT= $(top_builddir)/src/java
EYEDB_JAR=@EYEDB_JAR@
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR)

EYEDB_BINDIR=@EYEDB_BINDIR@
ODL_PACKAGE= org.eyedb.benchmark.polepos.teams.eyedb.data
EYEDBODLJAVA= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLJAVAFLAGS= --gencode=java --no-rootclass --package=$(ODL_PACKAGE) --output-dir=.
JAVA_ODL_FILES= B0.java B1.java B2.java B3.java B4.java IndexedPilot.java Pilot.java Database.java 
BUILT_SOURCES= $(JAVA_ODL_FILES)

$(JAVA_ODL_FILES): data.odl
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

CLEANFILES= $(JAVA_ODL_FILES)

EXTRA_DIST= data.odl
