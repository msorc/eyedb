SUBDIRS=org bench1 quicktour

JARFILE=eyedb-benchmarks.jar
noinst_DATA= $(JARFILE)

STAMP_FILE=classdist_package.stamp
JAR_DEPS=\
./org/eyedb/benchmark/$(STAMP_FILE) 

CLEANFILES=$(JARFILE)

JAR=@JAR@

$(JARFILE): $(JAR_DEPS)
	$(JAR) -cvf $(JARFILE) `find org/eyedb/benchmark -name "*.class"`
