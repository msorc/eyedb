packagedir= $(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour/eyedb/quicktour/collection
dist_package_JAVA= $(JAVA_ODL_FILES) 
JAVAROOT= $(top_builddir)/src/java
EYEDB_JAR=@EYEDB_JAR@
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR)

EYEDB_BINDIR=@EYEDB_BINDIR@
ODL_PACKAGE= org.eyedb.benchmark.quicktour.eyedb.quicktour.collection
EYEDBODLJAVA= $(EYEDB_BINDIR)/eyedbodl
CPPFLAGS=--cpp-flags="-UUSE_ARRAY -UUSE_INVERSE"
EYEDBODLJAVAFLAGS= $(CPPFLAGS) --gencode=java --no-rootclass --package=$(ODL_PACKAGE) --output-dir=.
JAVA_ODL_FILES= \
Course.java \
Database.java \
Person.java \
set_class_Course_ref.java \
set_class_Student_ref.java \
Student.java \
Teacher.java 
ODL_FILE=$(top_srcdir)/src/java/org/eyedb/benchmark/quicktour/eyedb/quicktour/quicktour.odl

BUILT_SOURCES= $(JAVA_ODL_FILES)

$(JAVA_ODL_FILES): $(ODL_FILE)
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

CLEANFILES= $(JAVA_ODL_FILES)

