eyedbconfdir=$(sysconfdir)/$(PACKAGE)

EXTRA_DIST=eyedb.conf Access

install-data-local:
	test -z "$(eyedbconfdir)" || $(mkdir_p) "$(DESTDIR)$(eyedbconfdir)"
	( \
		test ! -f $(eyedbconfdir)/eyedb.conf && $(INSTALL_DATA) eyedb.conf $(DESTDIR)$(eyedbconfdir)/eyedb.conf ; \
		test ! -f $(eyedbconfdir)/Access && $(INSTALL_DATA) $(srcdir)/Access $(DESTDIR)$(eyedbconfdir)/Access ; \
		exit 0 \
	)

