if COND_DOXYGEN
DOXYGEN_DIR= doxygen # for now, we don't compile and install doxygen doc
else
DOXYGEN_DIR= 
endif

if COND_XSLTPROC
MANPAGES_DIR= #manpages # for now, we don't compile and install man pages
else
MANPAGES_DIR= 
endif

if COND_LATEX
MANUAL_DIR= manual
else
MANUAL_DIR= 
endif


SUBDIRS=$(DOXYGEN_DIR) $(MANPAGES_DIR) $(MANUAL_DIR)
DIST_SUBDIRS=doxygen manpages manual

#htmldir=$(datadir)/doc/$(PACKAGE)
#html_DATA=index.html
