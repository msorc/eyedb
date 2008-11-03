noinst_DATA=$(JAR_FILE)

JAR_FILE=poleposition.jar
JAR_DEPS=$(system find $(POLEPOS_SRC_DIR)/src -name '*.java')

POLEPOS_SRC_DIR=src/polepos-oct-27-2008

$(JAR_FILE): $(JAR_DEPS)
	(cd $(POLEPOS_SRC_DIR); ant compile)
	(cd $(POLEPOS_SRC_DIR)/bin; jar cvf ../../../$(JAR_FILE) `find . -name '*.class'` )

CLEANFILES= $(JAR_FILE)
