if COND_EYEDB
EYEDB_SUBDIR=eyedb
else
EYEDB_SUBDIR=
endif

if COND_DB4O
DB4O_SUBDIR=db4o
else
DB4O_SUBDIR=
endif

if COND_HIBERNATE
HIBERNATE_SUBDIR=hibernate
else
HIBERNATE_SUBDIR=
endif

SUBDIRS=$(EYEDB_SUBDIR) $(DB4O_SUBDIR) $(HIBERNATE_SUBDIR)

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour
dist_package_JAVA =\
Quicktour.java

JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java

