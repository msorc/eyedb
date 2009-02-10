MANUAL=Installation

manualdir=$(datadir)/doc/$(PACKAGE)/manual/pdf
PDF_PREFIX=EyeDB
manual_DATA=$(PDF_PREFIX)$(MANUAL).pdf

XSL_PARAMS = $(top_srcdir)/doc/manual/docbook-common/params.xsl
DBLATEXFLAGS = -p $(XSL_PARAMS)

EyeDBInstallation.pdf: installation.xml
	$(DBLATEX) $(DBLATEXFLAGS) -o $@ $<

