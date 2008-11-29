HIBERNATE_JAR=@HIBERNATE_JAR@

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour/hibernate

dist_package_JAVA =\
HibernateCreateBenchmark.java \
Person.java \
Student.java \
Course.java \
Teacher.java

JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmark.jar:$(top_srcdir)/src/java:$(HIBERNATE_JAR)

EXTRA_DIST= hibernate.cfg.xml Course.hbm.xml Person.hbm.xml Student.hbm.xml Teacher.hbm.xml
