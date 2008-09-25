SUBDIRS=org
#bench1

JARFILE=eyedb-benchmarks.jar
noinst_DATA= $(JARFILE)

STAMP_FILE=classdist_package.stamp
JAR_DEPS=\
./org/eyedb/benchmark/quicktour/eyedb/$(STAMP_FILE) \
./org/eyedb/benchmark/framework/$(STAMP_FILE)

CLEANFILES=$(JARFILE)

JAR=@JAR@

$(JARFILE): $(JAR_DEPS)
	$(JAR) -cvf $(JARFILE) `find org/eyedb/benchmark -name "*.class"`
