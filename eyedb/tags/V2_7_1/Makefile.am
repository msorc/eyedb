# Warning: the directories order is important:
# - install must be done last because it needs one binary (eyedbcrypt) which is installed during make install in src/
SUBDIRS=include scripts src install 
DIST_SUBDIRS=doc examples include scripts src install tests

EXTRA_DIST= AUTHORS COPYING ChangeLog README VERSION autogen.sh

TAGS:
	find $(top_srcdir)/src $(top_srcdir)/include \! \( -name '*~' -o \( -type d -name '*.old' -prune \) \) \( -name '*.c' -o -name '*.c.in' -o -name '*.h' -o -name '*.h.in' -o -name '*.cc' -o -name '*.java' -o -name '*.java.in' -o -name 'Makefile.am' \) -print | etags -t -
.PHONY: TAGS

doc:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) all)

