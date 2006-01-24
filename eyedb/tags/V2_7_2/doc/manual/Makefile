#
# Makefile, EyeDB Package
#
# doc makefile
#
# Eric Viara Copyright (c) SYSRA Informatique 1997
#

TEXFILES = preface overview install getstart objmodel storman ODL c++api OQL \
	   methods indexes javaapi corba admin annexes

all: eyedb.ps html/eyedb.html
	(cd progm; $(MAKE) all)

.PHONY: installguide

installguide: installguide.tex $(FORCE)
	latex2html -split 0 -no_navigation -dir installguide -show_section_numbers $<
	./patch-html installguide/*.html

eyedb.dvi: eyedb.tex title.tex $(TEXFILES:%=progm/%.tex) $(FORCE)

html/eyedb.html: eyedb.tex title.tex $(TEXFILES:%=progm/%.tex) $(FORCE)
#	latex2html -split 3 -dir html -show_section_numbers $<
	latex2html -noaddress -split 3 -dir html $<
	sh -x ./patch-html
#	latex2html -split 0 -no_navigation -dir html -show_section_numbers $<
#	#(in only one file)

include ../Makefile.common

force:

clean::
	(cd progm; $(MAKE) clean)
	(cd refm; $(MAKE) clean)
	rm -rf html/*
