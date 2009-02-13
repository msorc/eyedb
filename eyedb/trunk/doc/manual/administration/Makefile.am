MANUAL=Administration
PDF_PREFIX=EyeDB
MANUAL_XML_FILES=database.xml server.xml user.xml

if COND_DBLATEX
pdfmanualdir=$(datadir)/doc/$(PACKAGE)/manual/pdf
pdfmanual_DATA=$(PDF_PREFIX)$(MANUAL).pdf
endif

XSL_PARAMS = $(top_srcdir)/doc/manual/docbook-common/params.xsl
DBLATEXFLAGS = -p $(XSL_PARAMS)

$(PDF_PREFIX)$(MANUAL).pdf: administration.xml
	$(DBLATEX) $(DBLATEXFLAGS) -o $@ $<

$(PDF_PREFIX)$(MANUAL).pdf: $(MANUAL_XML_FILES)

if COND_DOCBOOK_HTML
htmlmanualdir=$(datadir)/doc/$(PACKAGE)/manual/html/administration
htmlmanual_DATA=html/index.html
endif

XSLTPROC=@XSLTPROC@
XSLTPROCFLAGS=--novalid --nonet --xinclude
HTML_XSL_STYLESHEET=@HTML_XSL_STYLESHEET_DIR@/docbook.xsl

html/index.html: administration.xml
	test -d html || mkdir html
	$(XSLTPROC) $(XSLTPROCFLAGS) --output $@ $(HTML_XSL_STYLESHEET) $<

html/index.html: $(MANUAL_XML_FILES)

clean-local:
	-rm -rf *.pdf html/
