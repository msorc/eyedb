MANUAL=Installation
PDF_PREFIX=EyeDB

if COND_DBLATEX
manualdir=$(datadir)/doc/$(PACKAGE)/manual/pdf
manual_DATA=$(PDF_PREFIX)$(MANUAL).pdf
endif

XSL_PARAMS = $(top_srcdir)/doc/manual/docbook-common/params.xsl
DBLATEXFLAGS = -p $(XSL_PARAMS)

$(PDF_PREFIX)$(MANUAL).pdf: installation.xml
	$(DBLATEX) $(DBLATEXFLAGS) -o $@ $<

