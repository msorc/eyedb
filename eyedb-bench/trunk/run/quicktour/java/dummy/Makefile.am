noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' \
        -e 's,@JAVA\@,$(JAVA),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

bench: runit.sh
	$(SHELL) -x runit.sh

CLEANFILES=$(noinst_DATA)
