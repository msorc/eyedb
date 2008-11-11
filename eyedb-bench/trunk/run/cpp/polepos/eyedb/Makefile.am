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

bench-local: runit.sh
	$(SHELL) runit.sh --local

bahrain-local: runit.sh
	$(SHELL) runit.sh --local bahrain

barcelona-local: runit.sh
	$(SHELL) runit.sh --local barcelona

imola-local: runit.sh
	$(SHELL) runit.sh --local imola

melbourne-local: runit.sh
	$(SHELL) runit.sh --local melbourne

sepang-local: runit.sh
	$(SHELL) runit.sh --local sepang

