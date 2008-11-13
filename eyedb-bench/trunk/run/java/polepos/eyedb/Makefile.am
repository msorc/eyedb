EYEDB_BINDIR=@EYEDB_BINDIR@
EYEDB_SBINDIR=@EYEDB_SBINDIR@
EYEDB_JAR=@EYEDB_JAR@
POLEPOS_DIR=@POLEPOS_DIR@

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_srcdir[@],$(top_srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' \
	-e 's,@POLEPOS_DIR[@],$(POLEPOS_DIR),g' \
	-e 's,@EYEDB_BINDIR\@,$(EYEDB_BINDIR),g' \
	-e 's,@EYEDB_SBINDIR\@,$(EYEDB_SBINDIR),g' \
	-e 's,@EYEDB_JAR\@,$(EYEDB_JAR),g' \
	-e 's,@JAVA\@,$(JAVA),g' 

noinst_DATA=runit.sh

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

bench: runit.sh
	$(SHELL) runit.sh

bahrain: runit.sh
	$(SHELL) runit.sh bahrain

barcelona: runit.sh
	$(SHELL) runit.sh barcelona

imola: runit.sh
	$(SHELL) runit.sh imola

melbourne: runit.sh
	$(SHELL) runit.sh melbourne

sepang: runit.sh
	$(SHELL) runit.sh sepang
