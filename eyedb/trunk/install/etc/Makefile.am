eyedbconfdir=$(sysconfdir)/$(PACKAGE)

EXTRA_DIST=eyedb.conf eyedbd.conf Access

edit = sed \
        -e 's,@datadir\@,$(pkgdatadir),g' \
        -e 's,@localstatedir\@,$(localstatedir),g' \
        -e 's,@prefix\@,$(prefix),g' \
        -e 's,@exec_prefix\@,$(exec_prefix),g'

# Not yet
#eyedb.conf: Makefile $(srcdir)/eyedb.conf.in
#        rm -f eyedb.conf eyedb.conf.tmp
#        $(edit) $(srcdir)/eyedb.conf.in >eyedb.conf.tmp
#        mv eyedb.conf.tmp eyedb.conf

install-data-local:
	test -z "$(eyedbconfdir)" || $(mkdir_p) "$(DESTDIR)$(eyedbconfdir)"
	( \
		test ! -f $(eyedbconfdir)/eyedb.conf && $(INSTALL_DATA) $(srcdir)/eyedb.conf $(DESTDIR)$(eyedbconfdir)/eyedb.conf ; \
		test ! -f $(eyedbconfdir)/eyedbd.conf && $(INSTALL_DATA) $(srcdir)/eyedbd.conf $(DESTDIR)$(eyedbconfdir)/eyedbd.conf ; \
		test ! -f $(eyedbconfdir)/Access && $(INSTALL_DATA) $(srcdir)/Access $(DESTDIR)$(eyedbconfdir)/Access ; \
		exit 0 \
	)

