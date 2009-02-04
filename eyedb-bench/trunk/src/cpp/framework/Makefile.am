noinst_LIBRARIES= libeyedbbenchmark.a

libeyedbbenchmark_a_SOURCES= \
benchmark.cc \
properties.cc \
reporter.cc \
stopwatch.cc

check_PROGRAMS=test_stopwatch test_benchmark

test_stopwatch_SOURCES=test_stopwatch.cc
test_stopwatch_LDADD= $(builddir)/libeyedbbenchmark.la

test_benchmark_SOURCES=test_benchmark.cc
test_benchmark_LDADD= $(builddir)/libeyedbbenchmark.la

