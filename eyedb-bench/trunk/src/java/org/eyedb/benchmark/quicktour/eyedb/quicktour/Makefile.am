packagedir= $(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour/eyedb/quicktour
dist_package_JAVA= $(JAVA_ODL_FILES) 
JAVAROOT= $(top_builddir)/src/java
EYEDB_JAR=@EYEDB_JAR@
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR)

EYEDB_BINDIR=@EYEDB_BINDIR@
ODL_PACKAGE= org.eyedb.benchmark.quicktour.eyedb.quicktour
EYEDBODLJAVA= $(EYEDB_BINDIR)/eyedbodl
EYEDBODLJAVAFLAGS= --gencode=java --no-rootclass --package=$(ODL_PACKAGE) --output-dir=.
JAVA_ODL_FILES= \
Course.java \
Database.java \
Person.java \
set_class_Course_ref.java \
set_class_Student_ref.java \
Student.java \
Teacher.java 

BUILT_SOURCES= $(JAVA_ODL_FILES)

$(JAVA_ODL_FILES): quicktour.odl
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

CLEANFILES= $(JAVA_ODL_FILES)

EXTRA_DIST= quicktour.odl
