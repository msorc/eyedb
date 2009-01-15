noinst_DATA=runit.sh

EDIT = sed \
	-e 's,@srcdir[@],$(srcdir),g' \
	-e 's,@top_srcdir[@],$(top_srcdir),g' \
	-e 's,@top_builddir[@],$(top_builddir),g' 

%.sh: $(srcdir)/%.sh.in
	rm -f $*.sh $*.sh.tmp
	$(EDIT) $(srcdir)/$*.sh.in >$*.sh.tmp
	mv $*.sh.tmp $*.sh

runit.sh: Makefile

bench: runit.sh
	$(SHELL) runit.sh

bench-local: runit.sh
	$(SHELL) runit.sh -Dmode=local

