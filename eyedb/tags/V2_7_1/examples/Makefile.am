if COND_JAVA
JAVA_SRC_DIR= JavaBinding
else
JAVA_SRC_DIR= 
endif

SUBDIRS=GettingStarted C++Binding $(JAVA_SRC_DIR) oql

eyedbexamplesdir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/examples
dist_eyedbexamples_DATA=Makefile.eyedb-examples

install-data-hook:
	cd $(DESTDIR)/$(eyedbexamplesdir) ; \
	for f in `find . -name Makefile.eyedb-examples` ; \
	do \
		mv $$f `dirname $$f`/Makefile ; \
	done

