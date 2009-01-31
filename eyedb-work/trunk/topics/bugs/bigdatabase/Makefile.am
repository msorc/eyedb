# default value
# best way is to pass it to make as in:
# make run DATABASE_DIR=/data1/ddd
DATABASE_DIR=/var/tmp

TOPIC_SCRIPT_FILES = test-init-eyedb.sh

noinst_DATA=$(TOPIC_SCRIPT_FILES)
EDIT = sed \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_LIBDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

$(TOPIC_SCRIPT_FILES): Makefile

run: test-init-eyedb.sh
	$(SHELL) -x test-init-eyedb.sh $(DATABASE_DIR)