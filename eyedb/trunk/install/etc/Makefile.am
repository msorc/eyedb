eyedbconfdir=$(sysconfdir)/$(PACKAGE)

EXTRA_DIST=eyedb.conf eyedbd.conf Access

install-data-local:
	test -z "$(eyedbconfdir)" || $(mkdir_p) "$(DESTDIR)$(eyedbconfdir)"
	( \
		test ! -f $(eyedbconfdir)/eyedb.conf && $(INSTALL_DATA) $(srcdir)/eyedb.conf $(DESTDIR)$(eyedbconfdir)/eyedb.conf ; \
		test ! -f $(eyedbconfdir)/eyedbd.conf && $(INSTALL_DATA) $(srcdir)/eyedbd.conf $(DESTDIR)$(eyedbconfdir)/eyedbd.conf ; \
		test ! -f $(eyedbconfdir)/Access && $(INSTALL_DATA) $(srcdir)/Access $(DESTDIR)$(eyedbconfdir)/Access ; \
		exit 0 \
	)

