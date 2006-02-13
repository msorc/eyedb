eyedbconfdir=$(sysconfdir)/$(PACKAGE)
noinst_DATA= eyedb.conf eyedbd.conf

EDIT = sed \
        -e 's,@bindir\@,$(bindir),g' \
        -e 's,@exec_prefix\@,$(exec_prefix),g' \
        -e 's,@datadir\@,$(pkgdatadir),g' \
        -e 's,@libdir\@,$(libdir),g' \
        -e 's,@localstatedir\@,$(localstatedir),g' \
        -e 's,@prefix\@,$(prefix),g' \
        -e 's,@sysconfdir\@,$(sysconfdir),g'

eyedb.conf: Makefile $(srcdir)/eyedb.conf.in
eyedbd.conf: Makefile $(srcdir)/eyedbd.conf.in

%.conf: Makefile $(srcdir)/%.conf.in
	rm -f $*.conf $*.conf.tmp
	$(EDIT) $(srcdir)/$*.conf.in >$*.conf.tmp
	mv $*.conf.tmp $*.conf

install-data-local:
	test -z "$(eyedbconfdir)" || $(mkdir_p) "$(DESTDIR)$(eyedbconfdir)"
	( \
		test ! -f $(eyedbconfdir)/eyedb.conf && $(INSTALL_DATA) eyedb.conf $(DESTDIR)$(eyedbconfdir)/eyedb.conf ; \
		test ! -f $(eyedbconfdir)/eyedbd.conf && $(INSTALL_DATA) eyedbd.conf $(DESTDIR)$(eyedbconfdir)/eyedbd.conf ; \
		test ! -f $(eyedbconfdir)/Access && $(INSTALL_DATA) $(srcdir)/Access $(DESTDIR)$(eyedbconfdir)/Access ; \
		exit 0 \
	)

EXTRA_DIST=eyedb.conf.in eyedbd.conf.in Access
