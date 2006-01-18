if COND_JAVA
JAVA_SRC_DIR= JavaBinding
else
JAVA_SRC_DIR= 
endif

SUBDIRS=GettingStarted C++Binding $(JAVA_SRC_DIR) oql

pkgdata_DATA=Makefile.eyedb

eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)/examples
dist_eyedbexamples_DATA=Makefile.eyedb-examples README

install-data-hook:
	cd $(DESTDIR)/$(eyedbexamplesdir) ; \
	for f in `find . -name Makefile.eyedb-examples` ; \
	do \
		mv $$f `dirname $$f`/Makefile ; \
	done

