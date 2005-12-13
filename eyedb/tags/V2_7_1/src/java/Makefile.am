SUBDIRS=src

jardir=$(pkglibdir)/java
JARFILE=eyedb.jar
jar_DATA = $(JARFILE)

STAMP_FILE=classdist_package.stamp
JAR_DEPS=\
./src/org/eyedb/dbm/$(STAMP_FILE) \
./src/org/eyedb/syscls/$(STAMP_FILE) \
./src/org/eyedb/utils/$(STAMP_FILE) \
./src/org/eyedb/$(STAMP_FILE)

CLEANFILES=$(JARFILE)

JAR=@JAR@

$(JARFILE): $(JAR_DEPS)
	cd src ; $(JAR) -cvf ../$(JARFILE) `find . -name "*.class"`;

#install-data-hook:
#	-cd $(DESTDIR)$(pkglibdir)/java; find . -name "*.class" -exec rm {} \;

### This does not work yet... to be finished, however, there is one big problem:
### generated Java code is not in ONE file as in C++, and the list of files is 
### known from the ODL code...
# ODL->Java
#EYEDBODL_exe=$(top_builddir)/src/eyedb/eyedbodl
EYEDBODL_exe=$(bindir)/eyedbodl

EYEDBODL = $(EYEDBODL_exe) --gencode=java --output-dir=$(top_srcdir)/src/java/src

EYEDBODLFLAGS_dbm = --package=org.eyedb.dbm \
        --schema-name=eyedbdbm --attr-style=implicit --no-rootclass

$(top_srcdir)/src/eyedb/dbm.cc $(top_srcdir)/src/eyedb/dbm.h: DBM.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS_dbm) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found, using $*.cc old C++ code; touch $*.cc ; \
	fi

EYEDBODLFLAGS_syscls = --cpp-flags=-DCOMPLETE=0 --export \
        --package=org.eyedb.syscls --schema-name=eyedb --attr-style=explicit \
        --down-casting=yes --class-enums=no --no-rootclass \
        --system-class

$(top_srcdir)/src/eyedb/syscls.cc $(top_srcdir)/src/eyedb/syscls.h: syscls.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS_syscls) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found, using $*.cc old C++ code; touch $*.cc ; \
	fi

EYEDBODLFLAGS_utils = --export --package=utils \
        --schema-name=eyedb --attr-style=explicit --down-casting=yes \
        --class-enums=yes --no-rootclass --system-class \
        --error-policy=exception

$(top_srcdir)/src/eyedb/utils.cc $(top_srcdir)/src/eyedb/utils.h: utils.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS_utils) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found, using $*.cc old C++ code; touch $*.cc ; \
	fi
