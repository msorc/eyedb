noinst_htmldir=$(datadir)/doc/$(PACKAGE)-$(VERSION)/api
noinst_html_DATA= html/index.html

DOXYGEN=@DOXYGEN@

$(noinst_html_DATA): eyedb-doxygen.cfg
	$(DOXYGEN) eyedb-doxygen.cfg
	for f in html/*.html ; \
	do \
		sed -e 's/ Eyedb/ eyedb/g' -e 's/>Eyedb/>eyedb/g' -e 's/ Eyedbsm/ eyedbsm/g' -e 's/>Eyedbsm/>eyedbsm/g' -e 's/ Eyedblib/ eyedblib/g' -e 's/>Eyedblib/>eyedblib/g' $$f > $$f.out ; mv $$f.out $$f ; \
	done

install-data-hook:
	test -z "$(noinst_htmldir)" || $(mkdir_p) "$(DESTDIR)$(noinst_htmldir)"
	( cd html ; \
	  for f in *.html *.css *.png ; do \
	    $(INSTALL_DATA) $$f "$(DESTDIR)$(noinst_htmldir)/$$p" ; \
	  done)

EXTRA_DIST=eyedb-doxygen.cfg
