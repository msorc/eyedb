DB4O_JAR=@DB4O_JAR@

noinst_DATA=runit.sh runit-local.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' \
        -e 's,@DB4O_JAR\@,$(DB4O_JAR),g' \
        -e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile
runit-local.sh: Makefile

bench: runit-local.sh
	$(SHELL) -x runit-local.sh

