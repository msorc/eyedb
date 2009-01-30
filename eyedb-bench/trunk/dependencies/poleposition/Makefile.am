noinst_DATA=$(JAR_FILE)

POLEPOS_SRC_DIR=$(top_srcdir)/dependencies/poleposition/
JAR_FILE=src/polepos-$(POLEPOS_VERSION)/poleposition.jar
JAR_DEPS=$(system find $(POLEPOS_SRC_DIR)/src/polepos-$(POLEPOS_VERSION)/src -name '*.java')

$(JAR_FILE): $(JAR_DEPS)
	$(ANT) -Dbasedir=$(POLEPOS_SRC_DIR)/src/polepos-$(POLEPOS_VERSION) -Dsrcdir=$(POLEPOS_SRC_DIR)/src/polepos-$(POLEPOS_VERSION)/src -buildfile $(POLEPOS_SRC_DIR)/build.xml jar

CLEANFILES= $(JAR_FILE)
