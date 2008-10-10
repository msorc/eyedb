lib_LTLIBRARIES= libeyedbbenchmark.la

libeyedbbenchmark_la_SOURCES= \
benchmark.cc \
stopwatch.cc

check_PROGRAMS=test_stopwatch test_benchmark

test_stopwatch_SOURCES=test_stopwatch.cc
test_stopwatch_LDADD= $(builddir)/libeyedbbenchmark.la

test_benchmark_SOURCES=test_benchmark.cc
test_benchmark_LDADD= $(builddir)/libeyedbbenchmark.la

