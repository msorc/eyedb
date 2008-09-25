packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/polepos/teams/eyedb

dist_package_JAVA =\
BahrainEyeDB.java \
BarcelonaEyeDB.java \
EyeDBCar.java \
EyeDBDriver.java \
EyeDBTeam.java \
ImolaEyeDB.java \
MelbourneEyeDB.java \
SepangEyeDB.java \
$(JAVA_ODL_FILES)

POLEPOS_JAR=@POLEPOS_JAR@
JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR):$(POLEPOS_JAR)

EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDB_SBINDIR=@EYEDB_SBINDIR@
EYEDB_JAR=@EYEDB_JAR@

ODL_DIR=barcelona
ODL_PACKAGE=org.eyedb.benchmark.polepos.teams.eyedb.$(ODL_DIR)
EYEDBODLJAVA = $(EYEDB_BINDIR)/eyedbodl --gencode=java
EYEDBODLJAVAFLAGS= --no-rootclass --package=$(ODL_PACKAGE) --output-dir=$(ODL_DIR)

JAVA_ODL_FILES=$(addprefix $(ODL_DIR)/, \
B0.java \
B1.java \
B2.java \
B3.java \
B4.java \
)

$(JAVA_ODL_FILES): barcelona.odl
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<


clean-local:
	-rm -f $(JAVA_ODL_FILES)
	-rm -f $(ODL_PACKAGE)/*.class

EXTRA_DIST= barcelona.odl

# ====================== TO BE MOVED

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

TESTS_ENVIRONMENT=$(SHELL)
TESTS=runit.sh

#noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' 

runit.sh: Makefile

