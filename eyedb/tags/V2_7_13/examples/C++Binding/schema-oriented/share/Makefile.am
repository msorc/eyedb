eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/examples/C++Binding/schema-oriented/share
dist_eyedbexamples_DATA=init.sh schema.odl
eyedbexamples_DATA=Makefile.eyedb-examples

check_LTLIBRARIES= libperson.la

nodist_libperson_la_SOURCES= person.cc personstubsbe.cc personstubsfe.cc

# The last directory is needed for endian.h, which is generated by configure.in
INCLUDES=-I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_builddir)/include

EYEDBODL_exe=$(bindir)/eyedbodl
EYEDBODL = $(EYEDBODL_exe) --gencode=C++
EYEDBODLFLAGS= --package=person --schema-name=person

person.cc person.h : schema.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found ; \
	fi

init:
	sh $(srcdir)/init.sh $(srcdir)/schema.odl
.PHONY: init

# Dependencies
person.$(OBJEXT): person.h

