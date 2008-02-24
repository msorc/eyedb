if COND_DOXYGEN
DOXYGEN_DIR= doxygen # for now, we don't compile and install doxygen doc
else
DOXYGEN_DIR= 
endif

if COND_JAVADOC
JAVADOC_DIR= javadoc
else
JAVADOC_DIR= 
endif

if COND_DOCBOOK_MANPAGES
MANPAGES_DIR= manpages 
else
MANPAGES_DIR= 
endif

if COND_LATEX
MANUAL_DIR= manual
else
MANUAL_DIR= 
endif

SUBDIRS=$(DOXYGEN_DIR) $(JAVADOC_DIR) $(MANPAGES_DIR) $(MANUAL_DIR) images release-notes
DIST_SUBDIRS=doxygen javadoc manpages manual images release-notes

htmldir=$(datadir)/doc/$(PACKAGE)
dist_html_DATA=index.html eyedbdoc.css
