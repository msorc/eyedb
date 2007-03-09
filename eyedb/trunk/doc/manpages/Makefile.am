man1_MANS= \
eyedbadmin.1 \
eyedbdbaccess.1 \
eyedbdbcopy.1 \
eyedbdbcreate.1 \
eyedbdbdelete.1 \
eyedbdblist.1 \
eyedbdbmcreate.1 \
eyedbdbmove.1 \
eyedbdbrename.1 \
eyedbd.1 \
eyedbgetenv.1 \
eyedbodl.1 \
eyedboql.1 \
eyedbpasswd.1 \
eyedbrc.1 \
eyedbsmd.1 \
eyedbsysaccess.1 \
eyedbuseradd.1 \
eyedbuserdbaccess.1 \
eyedbuserdelete.1 \
eyedbuserlist.1 \
eyedbuserpasswd.1 \
eyedb-options.7 

XSLTPROC=@XSLTPROC@
MANPAGES_XSL_STYLESHEET=@MANPAGES_XSL_STYLESHEET_DIR@/docbook.xsl

%.1: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

%.7: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

EXTRA_DIST= \
eyedbadmin.xml \
eyedbdbaccess.xml \
eyedbdbcopy.xml \
eyedbdbcreate.xml \
eyedbdbdelete.xml \
eyedbdblist.xml \
eyedbdbmcreate.xml \
eyedbdbmove.xml \
eyedbdbrename.xml \
eyedbd.xml \
eyedbgetenv.xml \
eyedbodl.xml \
eyedboql.xml \
eyedbpasswd.xml \
eyedbrc.xml \
eyedbsmd.xml \
eyedbsysaccess.xml \
eyedbuseradd.xml \
eyedbuserdbaccess.xml \
eyedbuserdelete.xml \
eyedbuserlist.xml \
eyedbuserpasswd.xml \
eyedb-options.xml
