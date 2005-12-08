packagedir=$(pkglibdir)/java/classes/org/eyedb/utils
dist_package_JAVA = \
Database.java \
Date.java \
Image.java \
ImageType.java \
Month.java \
OString.java \
Root.java \
TimeInterval.java \
Time.java \
TimeStamp.java \
URL.java \
WConfig.java \
Weekday.java


JAVAROOT=$(top_builddir)/src/java/src
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java/src

EYEDBODL_exe=$(bindir)/eyedbodl

EYEDBODL = $(EYEDBODL_exe) --gencode=java --output-dir=$(top_srcdir)/src/java/src/org/eyedb/utils

EYEDBODLFLAGS_utils = --export --package=org.eyedb.utils \
	--schema-name=eyedb --attr-style=explicit --down-casting=yes \
        --class-enums=yes --system-class \
        --error-policy=exception

JAVA_ODL_FILES= \
$(top_srcdir)/src/java/src/org/eyedb/utils/Database.java 

$(JAVA_ODL_FILES): utils.odl
	$(EYEDBODL) $(EYEDBODLFLAGS_utils) $<


