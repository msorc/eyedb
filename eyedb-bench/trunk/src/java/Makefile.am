SUBDIRS=org
#bench1

JARFILE=eyedb-benchmark.jar
noinst_DATA= $(JARFILE)

if COND_OPENOFFICE
OPENOFFICE_SUBDIR=org/eyedb/benchmark/framework/reporter/openoffice
else
OPENOFFICE_SUBDIR=
endif

DIRS_TO_JAR= \
org/eyedb/benchmark/framework \
$(OPENOFFICE_SUBDIR) \
org/eyedb/benchmark/framework/reporter \
org/eyedb/benchmark/framework/reporter/csv \
org/eyedb/benchmark/framework/reporter/simple \
org/eyedb/benchmark/petshop/eyedb \
org/eyedb/benchmark/polepos \
org/eyedb/benchmark/polepos/teams/db4o \
org/eyedb/benchmark/polepos/teams/eyedb \
org/eyedb/benchmark/polepos/teams/eyedb/data \
org/eyedb/benchmark/quicktour \
org/eyedb/benchmark/quicktour/dummy \
org/eyedb/benchmark/quicktour/db4o \
org/eyedb/benchmark/quicktour/eyedb \
org/eyedb/benchmark/quicktour/eyedb/collection \
org/eyedb/benchmark/quicktour/eyedb/inverse \
org/eyedb/benchmark/quicktour/hibernate \
org/eyedb/benchmark/utils/db4o

OTHER_FILES_TO_JAR= \
org/eyedb/benchmark/quicktour/hibernate/Course.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Person.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Student.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Teacher.hbm.xml

JAR=@JAR@
STAMP_FILE=classdist_package.stamp
CLASSES_TO_JAR=$(addsuffix /*.class, $(DIRS_TO_JAR))
JAR_DEPS=$(addsuffix /$(STAMP_FILE), $(DIRS_TO_JAR)) $(OTHER_FILES_TO_JAR)

CLEANFILES=$(JARFILE)

$(JARFILE): $(JAR_DEPS) $(OTHER_FILES_TO_JAR)
	$(JAR) -cvf $(JARFILE) $(CLASSES_TO_JAR) $(OTHER_FILES_TO_JAR)

