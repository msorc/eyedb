DIST_SUBDIRS=figures

manualdir=$(datadir)/doc/$(PACKAGE)/manual/pdf
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
LATEX_INPUTS=start.tex version.tex title.tex
DVIPS= TEXPICTS=$(srcdir) @DVIPS@
DVIPSFLAGS= -f -Ppdf -G0
PS2PDF=@PS2PDF@
PS2PDFFLAGS= -dPDFsettings=/prepress 

noinst_htmldir=$(datadir)/doc/$(PACKAGE)/manual/html
if COND_LATEX2HTML
noinst_html_DATA= \
html/Administration/index.html \
html/CplusplusBinding/index.html \
html/GettingStarted/index.html \
html/JavaBinding/index.html \
html/ODL/index.html \
html/OQL/index.html \
html/Overview/index.html
endif

LATEX2HTML=@LATEX2HTML@
LATEX2HTMLFLAGS=-toc_depth 6 -top_navigation -verbosity 0 -local_icons -info '' -address 'EyeDB manual' -init_file latex2html-init 

%.dvi: %.tex
	$(LATEX) $<
	$(LATEX) $<
	if grep 'There were undefined references' $*.log > /dev/null ; then $(LATEX) $<; fi
.PRECIOUS: %.dvi

%.ps: %.dvi
	$(DVIPS) $(DVIPSFLAGS) $< -o $@

$(PDF_PREFIX)%.pdf: %.ps
	$(PS2PDF) $(PS2PDFFLAGS) $< $@

html/%/index.html: %.tex
	-test ! -d html && mkdir html ; test ! -d html/$* && mkdir html/$*
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -no_images -split +2 -dir html/$* $*.tex

html/OQL/index.html: OQL.tex
	-test ! -d html && mkdir html ; test ! -d html/OQL && mkdir html/OQL
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -no_images -split +3 -dir html/OQL OQL.tex

html/Overview/index.html: Overview.tex
	-test ! -d html && mkdir html ; test ! -d html/Overview && mkdir html/Overview
	$(LATEX2HTML) $(LATEX2HTMLFLAGS) -split +2 -dir html/Overview Overview.tex

Administration.dvi: $(LATEX_INPUTS)
CplusplusBinding.dvi: $(LATEX_INPUTS)
GettingStarted.dvi: $(LATEX_INPUTS)
JavaBinding.dvi: $(LATEX_INPUTS)
ODL.dvi: $(LATEX_INPUTS)
OQL.dvi: $(LATEX_INPUTS)
Overview.dvi: $(LATEX_INPUTS)

html/Administration/index.html: $(LATEX_INPUTS)
html/CplusplusBinding/index.html: $(LATEX_INPUTS)
html/GettingStarted/index.html: $(LATEX_INPUTS)
html/JavaBinding/index.html: $(LATEX_INPUTS)
html/ODL/index.html: $(LATEX_INPUTS)
html/OQL/index.html: $(LATEX_INPUTS)
html/Overview/index.html: $(LATEX_INPUTS)

clean-local:
	rm -f *.pdf *.ps *.dvi *.aux *.idx *.log *.toc
	rm -f html/*/*

if COND_LATEX2HTML
install-data-hook:
	test -z "$(noinst_htmldir)" || $(mkdir_p) "$(DESTDIR)$(noinst_htmldir)"
	-rm "$(DESTDIR)$(noinst_htmldir)/index.html"
	for d in Administration CplusplusBinding GettingStarted JavaBinding ODL OQL Overview ; \
	do \
		test -z "$(noinst_htmldir)/$$d" || $(mkdir_p) "$(DESTDIR)$(noinst_htmldir)/$$d" ; \
		(cd html/$$d ; \
			for f in *.html *.css *.png ; do \
				$(INSTALL_DATA) $$f "$(DESTDIR)$(noinst_htmldir)/$$d/$$f" ; \
			done ; \
		) ; \
	done
else
endif

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
