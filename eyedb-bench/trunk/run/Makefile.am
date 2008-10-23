SUBDIRS= cpp java

bench:
	for d in $(SUBDIRS) ; do \
		(cd $$d; $(MAKE) $(AM_MAKEFLAGS) $@) ; \
	done
