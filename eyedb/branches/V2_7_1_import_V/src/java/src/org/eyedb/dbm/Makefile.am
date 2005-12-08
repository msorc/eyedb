packagedir=$(pkglibdir)/java/classes/org/eyedb/dbm
dist_package_JAVA =				\
Database.java					\
DBAccessMode.java				\
DBEntry.java					\
DBProperty.java					\
DBPropertyValue.java				\
DBUserAccess.java				\
Root.java					\
SysAccessMode.java				\
SysUserAccess.java				\
UserEntry.java					\
UserType.java

JAVAROOT=$(top_builddir)/src/java/src
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java/src

EYEDBODL_exe=$(bindir)/eyedbodl

EYEDBODL = $(EYEDBODL_exe) --gencode=java --output-dir=$(top_srcdir)/src/java/src/org/eyedb/dbm

EYEDBODLFLAGS_utils = --export --package=org.eyedb.dbm \
        --schema-name=eyedb --attr-style=explicit --down-casting=yes \
        --class-enums=yes --system-class \
        --error-policy=exception

JAVA_ODL_FILES= \
$(top_srcdir)/src/java/src/org/eyedb/dbm/Database.java 

$(JAVA_ODL_FILES): DBM.odl
	$(EYEDBODL) $(EYEDBODLFLAGS_utils) $<
