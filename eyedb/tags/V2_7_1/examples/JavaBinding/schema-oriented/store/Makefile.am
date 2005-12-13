eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/examples/JavaBinding/schema-oriented/store
dist_eyedbexamples_DATA= PersonTest.java schema.odl Makefile.eyedb-examples

SUBDIRS=person

packagedir= $(pkglibdir)/java/classes/org/eyedb/examples/schema-oriented/store
package_JAVA=PersonTest.java 

JAVAROOT=$(top_builddir)/examples/JavaBinding/schema-oriented/store
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb.jar:$(top_builddir)/examples/JavaBinding/schema-oriented/store
