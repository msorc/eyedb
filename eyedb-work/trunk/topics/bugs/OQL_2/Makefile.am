# ######################################################################
# Editable part

# Binaries
# Example:
# TOPIC_BINARY_FILES = test1
TOPIC_BINARY_FILES = 

# For each binary listed in $(TOPIC_BINARY_FILES) add the following line:
# <<BINARY_FILE>>_SOURCES=<<list of all C++ source files>>
# Example:
# test1_SOURCES = test1.cc

# Java files (excluding the .java files generated from odl)
# Example:
# TOPIC_JAVA_FILES = Test1.java
TOPIC_JAVA_FILES = 

# ODL file (only one file allowed)
# Example:
# TOPIC_ODL_FILE = person.odl
TOPIC_ODL_FILE = 

# Script files
# Example:
# TOPIC_SCRIPT_FILES = init.sh
# (this implies that you have a init.sh.in source script)
TOPIC_SCRIPT_FILES = 

# End of editable part
# ######################################################################

TOPIC = bugs/OQL_2

# ######################################################################
#
# Common definitions
#
TOPIC_ODL_NO_SUFFIX=$(subst .odl,,$(TOPIC_ODL_FILE))
BUILT_SOURCES= $(if $(TOPIC_ODL_FILE),$(TOPIC_ODL_NO_SUFFIX).cc $(TOPIC_ODL_NO_SUFFIX).h $(TOPIC_ODL_NO_SUFFIX)/Database.java,)
OTHER_BUILD_FILES=$(if $(TOPIC_ODL_FILE),$(TOPIC_ODL_NO_SUFFIX)mthbe-skel.cc $(TOPIC_ODL_NO_SUFFIX)mthfe-skel.cc $(TOPIC_ODL_NO_SUFFIX)stubsbe.cc $(TOPIC_ODL_NO_SUFFIX)stubsfe.cc template_$(TOPIC_ODL_NO_SUFFIX).cc Makefile.$(TOPIC_ODL_NO_SUFFIX) $(TOPIC_ODL_NO_SUFFIX)/*.java)
CLEANFILES=$(BUILT_SOURCES) $(OTHER_BUILD_FILES)

#
# Programs
#
noinst_PROGRAMS= $(TOPIC_BINARY_FILES)
INCLUDES=$(EYEDB_CFLAGS)
LDADD=$(if $(TOPIC_ODL_FILE),$(TOPIC_ODL_NO_SUFFIX).o,) $(EYEDB_LIBS)

#
# Java classes
#
noinst_JAVA=$(TOPIC_JAVA_FILES)
EYEDB_JAR=$(EYEDB_LIBDIR)/eyedb/java/eyedb.jar
JAVAROOT=$(top_builddir)/src/$(TOPIC)
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/$(TOPIC):$(EYEDB_JAR)

# 
# ODL -> Java
#
EYEDBODLJAVA = $(EYEDB_BINDIR)/eyedbodl --gencode=java
EYEDBODLJAVA_FLAGS= --no-rootclass 

$(TOPIC_ODL_NO_SUFFIX)/Database.java : $(TOPIC_ODL_FILE)
	$(EYEDBODLJAVA) $(EYEDBODLJAVA_FLAGS) --package=$(TOPIC_ODL_NO_SUFFIX) $<

# 
# ODL -> C++
# 
EYEDBODLCXX = $(EYEDB_BINDIR)/eyedbodl --gencode=C++
EYEDBODLCXX_FLAGS= 

%.cc %.h : %.odl
	$(EYEDBODLCXX) $(EYEDBODLCXX_FLAGS) $<

#
# Scripts
#
noinst_DATA=$(TOPIC_SCRIPT_FILES)
EDIT = sed \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_LIBDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

$(TOPIC_SCRIPT_FILES): Makefile

# ######################################################################
