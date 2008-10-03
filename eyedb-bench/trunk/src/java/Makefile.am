SUBDIRS=org
#bench1

JARFILE=eyedb-benchmarks.jar
noinst_DATA= $(JARFILE)

STAMP_FILE=classdist_package.stamp
JAR_DEPS=\
./org/eyedb/benchmark/quicktour/eyedb/$(STAMP_FILE) \
./org/eyedb/benchmark/framework/$(STAMP_FILE) \
./org/eyedb/benchmark/polepos/$(STAMP_FILE) \
./org/eyedb/benchmark/polepos/teams/eyedb/data/$(STAMP_FILE) \
./org/eyedb/benchmark/polepos/teams/eyedb/$(STAMP_FILE)

OTHER_FILES_TO_JAR= \
org/eyedb/benchmark/quicktour/hibernate/Course.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Person.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Student.hbm.xml \
org/eyedb/benchmark/quicktour/hibernate/Teacher.hbm.xml

CLEANFILES=$(JARFILE)

JAR=@JAR@

$(JARFILE): $(JAR_DEPS)
	$(JAR) -cvf $(JARFILE) `find org/eyedb/benchmark -name "*.class"` $(OTHER_FILES_TO_JAR)

