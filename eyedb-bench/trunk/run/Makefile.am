if COND_POLEPOS
POLEPOS_SUBDIR=polepos
else
POLEPOS_SUBDIR=
endif

SUBDIRS= $(POLEPOS_SUBDIR) quicktour

bench:
	for d in $(SUBDIRS) ; do \
		(cd $$d; $(MAKE) $(AM_MAKEFLAGS) $@) ; \
	done
