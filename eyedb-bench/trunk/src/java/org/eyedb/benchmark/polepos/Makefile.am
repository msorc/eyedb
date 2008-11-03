SUBDIRS=teams

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/polepos
dist_package_JAVA =\
Racer.java \
RunSeason.java \
RunSeasonDb4o.java \
RunSeasonDb4oMod.java \
RunSeasonEyeDB.java \
RunSeasonHibernate.java \
SimpleReporter.java 

POLEPOS_DIR=@POLEPOS_DIR@
EYEDB_JAR=@EYEDB_JAR@
JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR):$(POLEPOS_DIR)/bin
