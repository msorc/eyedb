if COND_DOC
DOC_SRC_DIR=doc
else
DOC_SRC_DIR=
endif
SUBDIRS=src include scripts install pkg-config examples tests $(DOC_SRC_DIR) 

docdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)
doc_DATA=AUTHORS COPYING README

EXTRA_DIST= AUTHORS COPYING ChangeLog README VERSION autogen.sh

TAGS:
	find $(top_srcdir) \! -name '*~' \( -name '*.c' -o -name '*.h' -o -name '*.cc' -o -name '*.java' -o -name 'Makefile.am' -o \( -name '*.in' -a \! -name Makefile.in \) \) -print | etags -t -
.PHONY: TAGS

doc:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) all)

check-ascii:
	( \
		cd $(top_srcdir) ; \
		for f in `find . -name '*.java'` ; \
		do \
			echo $$f ; \
			iconv --to-code=ASCII -c --output=- $$f | diff --brief - $$f ; \
			if test $$? != 0 ; then echo $$f not ASCII; exit 1; fi ; \
		done ; \
		echo No non-ASCII java file found ; \
	)

dist-hook: check-ascii

doc-archive:
	-rm -rf /var/tmp/eyedb-doc-$(EYEDB_VERSION)
	cp -r $(datadir)/doc/eyedb /var/tmp/eyedb-doc-$(EYEDB_VERSION)
	tar cvzf eyedb-doc-$(EYEDB_VERSION).tar.gz -C /var/tmp eyedb-doc-$(EYEDB_VERSION)
	-rm -rf /var/tmp/eyedb-doc-$(EYEDB_VERSION)

AUTOMAKE_OPTIONS=dist-bzip2
