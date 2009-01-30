HIBERNATE_JAR=@HIBERNATE_JAR@

packagedir=$(pkglibdir)/java/classes/org/eyedb/benchmark/quicktour/hibernate

dist_package_JAVA =\
HibernateQuicktour.java \
Person.java \
Student.java \
Course.java \
Teacher.java

MAPPING_FILES= \
Course.hbm.xml \
Person.hbm.xml \
Student.hbm.xml \
Teacher.hbm.xml

JAVAROOT=$(top_builddir)/src/java
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb-benchmark.jar:$(top_srcdir)/src/java:$(HIBERNATE_JAR)

mapping.stamp: $(MAPPING_FILES)
	@for file in $(MAPPING_FILES); do \
		if test ! -f $(builddir)/$$file ; then \
			echo cp $(srcdir)/$$file $(builddir); cp $(srcdir)/$$file $(builddir); \
		elif diff $(srcdir)/$$file $(builddir)/$$file > /dev/null ; then \
			: ; \
		else \
			echo cp $(srcdir)/$$file $(builddir); cp $(srcdir)/$$file $(builddir); \
		fi ; \
	done
	echo timestamp > mapping.stamp

all-local: mapping.stamp

clean-local:
	-rm mapping.stamp

EXTRA_DIST= hibernate.cfg.xml $(MAPPING_FILES)
