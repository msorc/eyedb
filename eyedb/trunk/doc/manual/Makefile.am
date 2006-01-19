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
LATEX2HTMLCOMMONFLAGS=-toc_depth 6 -top_navigation -verbosity 0 -local_icons -dir html -init_file latex2html-init -address "`finger -l | grep Name | sed -e 's/Login.*Name: //' | iconv --from-code='UTF-8' --to-code='ISO_8859-1' -`"
LATEX2HTMLFLAGS=$(LATEX2HTMLCOMMONFLAGS) -no_images 

%.dvi: %.tex
	$(LATEX) $<
	$(LATEX) $<
	if grep 'There were undefined references' $*.log > /dev/null ; then $(LATEX) $<; fi
.PRECIOUS: %.dvi

%.ps: %.dvi
	$(DVIPS) $(DVIPSFLAGS) $< -o $@

$(PDF_PREFIX)%.pdf: %.ps
	$(PS2PDF) $(PS2PDFFLAGS) $< $@

html/%.html: %.tex
	-test ! -d html && mkdir html
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -split +2 -prefix $* $*.tex

html/OQL.html: OQL.tex
	-test ! -d html && mkdir html
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -split +3 -prefix OQL OQL.tex

html/Overview.html: Overview.tex
	-test ! -d html && mkdir html
	$(LATEX2HTML) $(LATEX2HTMLCOMMONFLAGS) -split +2 -prefix Overview Overview.tex

Administration.dvi: start.tex version.tex title.tex
CplusplusBinding.dvi: start.tex version.tex title.tex
GettingStarted.dvi: start.tex version.tex title.tex
JavaBinding.dvi: start.tex version.tex title.tex
ODL.dvi: start.tex version.tex title.tex
OQL.dvi: start.tex version.tex title.tex
Overview.dvi: start.tex version.tex title.tex

html/Administration.html: start.tex version.tex title.tex
html/CplusplusBinding.html: start.tex version.tex title.tex
html/GettingStarted.html: start.tex version.tex title.tex
html/JavaBinding.html: start.tex version.tex title.tex
html/ODL.html: start.tex version.tex title.tex
html/OQL.html: start.tex version.tex title.tex
html/Overview.html: start.tex version.tex title.tex

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
