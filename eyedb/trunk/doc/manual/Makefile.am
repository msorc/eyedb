SUBDIRS=figures

manualdir=$(datadir)/doc/$(PACKAGE)/manual

PDF_PREFIX=EyeDB

manual_DATA=\
$(PDF_PREFIX)Administration.pdf \
$(PDF_PREFIX)CplusplusBinding.pdf \
$(PDF_PREFIX)GettingStarted.pdf \
$(PDF_PREFIX)JavaBinding.pdf \
$(PDF_PREFIX)ODL.pdf \
$(PDF_PREFIX)OQL.pdf \
$(PDF_PREFIX)Overview.pdf

LATEX= TEXINPUTS=.:$(srcdir): @LATEX@
DVIPS= TEXPICTS=$(srcdir) @DVIPS@
DVIPSFLAGS= -f -Ppdf -G0
PS2PDF=@PS2PDF@
PS2PDFFLAGS= -dPDFsettings=/prepress 

noinst_htmldir=$(datadir)/doc/$(PACKAGE)/manual/html

if COND_LATEX2HTML
noinst_html_DATA= \
html/Administration.html \
html/CplusplusBinding.html \
html/GettingStarted.html \
html/JavaBinding.html \
html/ODL.html \
html/OQL.html \
html/Overview.html
endif

LATEX2HTML=@LATEX2HTML@
LATEX2HTMLFLAGS=-split +2 -toc_depth 6 -top_navigation -verbosity 0 -no_images -dir html -init_file latex2html-init

%.dvi: %.tex
	$(LATEX) $<
	$(LATEX) $<
	if grep 'There were undefined references' $*.log > /dev/null ; then $(LATEX) $<; fi
.PRECIOUS: %.dvi

%.ps: %.dvi
	$(DVIPS) $(DVIPSFLAGS) $< -o $@

$(PDF_PREFIX)%.pdf: %.ps
	$(PS2PDF) $(PS2PDFFLAGS) $< $@

GettingStarted.dvi: start.tex version.tex title.tex

html/%.html: %.tex
	-test ! -d html && mkdir html
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -prefix $* $*.tex

clean-local:
	rm -f *.pdf *.ps *.dvi *.aux *.idx *.log *.toc
	rm -f html/*

install-data-hook:
	test -z "$(noinst_htmldir)" || $(mkdir_p) "$(DESTDIR)$(noinst_htmldir)"
	-test -d html && ( cd html ; \
	  for f in *.html *.css *.png ; do \
	    $(INSTALL_DATA) $$f "$(DESTDIR)$(noinst_htmldir)/$$f" ; \
	  done)

EXTRA_DIST=\
start.tex \
title.tex \
Administration.tex \
CplusplusBinding.tex \
GettingStarted.tex \
JavaBinding.tex \
ODL.tex \
OQL.tex \
Overview.tex
