SUBDIRS=org
#bench1

JARFILE=eyedb-benchmarks.jar
noinst_DATA= $(JARFILE)

DIRS_TO_JAR= \
org/eyedb/benchmark/framework \
org/eyedb/benchmark/polepos \
org/eyedb/benchmark/polepos/teams/eyedb/data \
org/eyedb/benchmark/polepos/teams/eyedb \
org/eyedb/benchmark/quicktour/eyedb \
org/eyedb/benchmark/utils/db4o

OTHER_FILES_TO_JAR= \
org/eyedb/benchmark/quicktour/hibernate/Course.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Person.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Student.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Teacher.hbm.xml

JAR=@JAR@
STAMP_FILE=classdist_package.stamp
JAR_DEPS=$(addsuffix /$(STAMP_FILE), $(DIRS_TO_JAR))
CLASSES_TO_JAR=$(addsuffix /*.class, $(DIRS_TO_JAR))

CLEANFILES=$(JARFILE)

$(JARFILE): $(JAR_DEPS)
	$(JAR) -cvf $(JARFILE) $(CLASSES_TO_JAR) $(OTHER_FILES_TO_JAR)

