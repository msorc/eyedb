TOPIC=jodl
noinst_JAVA=\
JODL.java \
JODLMain.java

JAVAROOT=$(top_builddir)/topics/$(TOPIC)/src
EYEDB_JAR=$(EYEDB_LIBDIR)/eyedb/java/eyedb.jar
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/topics/$(TOPIC)/src:$(EYEDB_JAR)
