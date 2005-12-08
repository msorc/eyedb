SUBDIRS=figures

manualdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/manual

PDF_PREFIX=EyeDB

manual_DATA=\
$(PDF_PREFIX)Administration.pdf \
$(PDF_PREFIX)C++Binding.pdf \
$(PDF_PREFIX)GettingStarted.pdf \
$(PDF_PREFIX)JavaBinding.pdf \
$(PDF_PREFIX)ODL.pdf \
$(PDF_PREFIX)OQL.pdf \
$(PDF_PREFIX)Overview.pdf

LATEX= TEXINPUTS=.:$(top_srcdir)/doc/manual: @LATEX@
DVIPS= TEXPICTS=$(top_srcdir)/doc/manual @DVIPS@
DVIPSFLAGS= -f -Ppdf -G0
PS2PDF=@PS2PDF@
PS2PDFFLAGS= -dPDFsettings=/prepress 

%.dvi: %.tex
	$(LATEX) $<
	$(LATEX) $<
	if grep 'There were undefined references' $*.log > /dev/null ; then $(LATEX) $<; fi
.PRECIOUS: %.dvi

%.ps: %.dvi
	$(DVIPS) $(DVIPSFLAGS) $< -o $@

$(PDF_PREFIX)%.pdf: %.ps
	$(PS2PDF) $(PS2PDFFLAGS) $< $@

clean-local:
	rm -f *.pdf *.ps *.dvi *.aux *.idx *.log *.toc

GettingStarted.dvi: start.tex version.tex title.tex

EXTRA_DIST=\
start.tex \
title.tex \
Administration.tex \
C++Binding.tex \
GettingStarted.tex \
JavaBinding.tex \
ODL.tex \
OQL.tex \
Overview.tex
