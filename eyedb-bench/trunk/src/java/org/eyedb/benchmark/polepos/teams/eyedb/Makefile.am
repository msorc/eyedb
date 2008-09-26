SUBDIRS=data

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/polepos/teams/eyedb
dist_package_JAVA =\
BahrainEyeDB.java \
BarcelonaEyeDB.java \
EyeDBCar.java \
EyeDBDriver.java \
EyeDBTeam.java \
ImolaEyeDB.java \
MelbourneEyeDB.java \
SepangEyeDB.java 

POLEPOS_JAR=@POLEPOS_JAR@
EYEDB_JAR=@EYEDB_JAR@
JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR):$(POLEPOS_JAR)





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

