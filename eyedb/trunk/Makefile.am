# Warning: the directories order is important:
# - install must be done last because it needs one binary (eyedbcrypt) which is installed during make install in src/
SUBDIRS=include scripts src install 
DIST_SUBDIRS=doc examples include scripts src install tests

EXTRA_DIST= AUTHORS COPYING ChangeLog README VERSION autogen.sh

TAGS:
	find $(top_srcdir) \! -name '*~' \( -name '*.c' -o -name '*.h' -o -name '*.cc' -o -name '*.java' -o -name 'Makefile.am' -o \( -name '*.in' -a \! -name Makefile.in \) \) -print | etags -t -
.PHONY: TAGS

doc:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) all)

