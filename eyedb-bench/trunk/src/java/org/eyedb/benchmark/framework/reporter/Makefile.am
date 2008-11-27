if COND_OPENOFFICE
OPENOFFICE_SUBDIR=openoffice
else
OPENOFFICE_SUBDIR=
endif

SUBDIRS= $(OPENOFFICE_SUBDIR) simple

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/framework/reporter

dist_package_JAVA =\
DelegateReporter.java \
ReporterFactory.java

JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java


