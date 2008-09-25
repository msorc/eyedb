POLEPOS_JAR=@POLEPOS_JAR@

EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDB_SBINDIR=@EYEDB_SBINDIR@
EYEDB_JAR=@EYEDB_JAR@

ODL_PACKAGE=polepos
EYEDBODLJAVA = $(EYEDB_BINDIR)/eyedbodl --gencode=java
EYEDBODLJAVAFLAGS= --no-rootclass --package=$(ODL_PACKAGE)

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/polepos/teams/eyedb

JAVA_ODL_FILES=$(addprefix $(ODL_PACKAGE)/, \
Course.java \
Database.java \
Person.java \
set_class_Course_ref.java \
set_class_Student_ref.java \
Student.java \
Teacher.java )

dist_package_JAVA =\
BahrainEyeDB.java \
BarcelonaEyeDB.java \
EyeDBCar.java \
EyeDBDriver.java \
EyeDBTeam.java \
ImolaEyeDB.java \
MelbourneEyeDB.java \
SepangEyeDB.java


JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR):$(POLEPOS_JAR)

TESTS_ENVIRONMENT=$(SHELL)
TESTS=runit.sh

#noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' 

$(JAVA_ODL_FILES): quicktour.odl
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

clean-local:
	-rm -f $(JAVA_ODL_FILES)
	-rm -f $(ODL_PACKAGE)/*.class

EXTRA_DIST= quicktour.odl
