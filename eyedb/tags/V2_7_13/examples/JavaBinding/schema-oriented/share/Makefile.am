eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/examples/JavaBinding/schema-oriented/share
dist_eyedbexamples_DATA= init.sh schema.odl
eyedbexamples_DATA=  Makefile.eyedb-examples

SUBDIRS=person

JAR=@JAR@
JARFILE=person.jar
CLEANFILES=$(JARFILE)

$(JARFILE): person/class.stamp
	$(JAR) -cvf $(JARFILE) `find person -name "*.class"`;

check-local: $(JARFILE)

init:
	sh $(srcdir)/init.sh $(srcdir)/schema.odl
.PHONY: init
