tmpdir=/var/tmp

tmp_LTLIBRARIES= libtest.la
libtest_la_SOURCES= libtest.cc
libtest_la_LDFLAGS= -release $(EYEDB_VERSION)

tmp_PROGRAMS=compiletest
compiletest_SOURCES=test.cc
compiletest_LDADD=$(tmp_LTLIBRARIES)
