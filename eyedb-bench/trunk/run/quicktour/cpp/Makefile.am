if COND_EYEDB
EYEDB_SUBDIR=eyedb
else
EYEDB_SUBDIR=
endif

SUBDIRS=$(EYEDB_SUBDIR) 

bench:
	for d in $(SUBDIRS) ; do \
		(cd $$d; $(MAKE) $(AM_MAKEFLAGS) $@) ; \
	done
