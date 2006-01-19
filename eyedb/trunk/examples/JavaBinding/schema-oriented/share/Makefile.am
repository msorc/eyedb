eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)/examples/JavaBinding/schema-oriented/share
dist_eyedbexamples_DATA= schema.odl Makefile.eyedb-examples

SUBDIRS=person

JAR=@JAR@
JARFILE=person.jar
CLEANFILES=$(JARFILE)

$(JARFILE): person/class.stamp
	$(JAR) -cvf $(JARFILE) `find person -name "*.class"`;

check-local: $(JARFILE)

init:
	sh init.sh
