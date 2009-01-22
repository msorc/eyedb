SUBDIRS=array collection inverse

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour/eyedb
dist_package_JAVA =\
EyeDBQuicktour.java \
EyeDBQuicktourCollection.java \
EyeDBQuicktourInverse.java

EYEDB_JAR=@EYEDB_JAR@
JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java:$(EYEDB_JAR)

EXTRA_DIST= quicktour.odl
