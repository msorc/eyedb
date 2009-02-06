noinst_DATA=$(JAR_FILE)

POLEPOS_SRC_DIR=$(top_srcdir)/dependencies/poleposition/
ANT_BASEDIR=$(builddir)/src/polepos-$(POLEPOS_VERSION)
JAR_FILE=$(builddir)/src/polepos-$(POLEPOS_VERSION)/poleposition.jar
JAR_DEPS=$(system find $(POLEPOS_SRC_DIR)/src/polepos-$(POLEPOS_VERSION)/src -name '*.java')

$(JAR_FILE): $(JAR_DEPS)
	test -d $(ANT_BASEDIR) || mkdir -p $(ANT_BASEDIR)
	$(ANT) -Dbasedir=$(ANT_BASEDIR) -Dsrcdir=$(POLEPOS_SRC_DIR)/src/polepos-$(POLEPOS_VERSION) -buildfile $(POLEPOS_SRC_DIR)/build.xml jar

CLEANFILES= $(JAR_FILE)
