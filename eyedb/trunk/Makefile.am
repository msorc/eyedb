if COND_DOC
DOC_SRC_DIR=doc
else
DOC_SRC_DIR=
endif
SUBDIRS=src include scripts install pkg-config examples tests $(DOC_SRC_DIR) 

docdir=$(datadir)/doc/$(PACKAGE)
doc_DATA=AUTHORS COPYING COPYING.LESSER README

EXTRA_DIST= AUTHORS COPYING COPYING.LESSER ChangeLog README VERSION autogen.sh

TAGS:
	find $(top_srcdir) \! -name '*~' \( -name '*.c' -o -name '*.h' -o -name '*.cc' -o -name '*.l' -o -name '*.y' -o -name '*.java' -o -name 'Makefile.am' -o \( -name '*.in' -a \! -name Makefile.in \) \) -print | etags -t -
.PHONY: TAGS

doc:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) all)
	(cd examples && $(MAKE) $(AM_MAKEFLAGS) all)
.PHONY: doc

doc-install:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) install)
	(cd examples && $(MAKE) $(AM_MAKEFLAGS) install)
.PHONY: doc-install

doc-export:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) doc-export)
.PHONY: doc-export

doc-archive:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) doc-archive)
.PHONY: doc-archive

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

AUTOMAKE_OPTIONS=dist-bzip2
