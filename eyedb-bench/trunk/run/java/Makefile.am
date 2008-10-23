SUBDIRS= polepos quicktour

bench:
	for d in $(SUBDIRS) ; do \
		(cd $$d; $(MAKE) $(AM_MAKEFLAGS) $@) ; \
	done
