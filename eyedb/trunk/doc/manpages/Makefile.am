man1_MANS= \
eyedb-options.1 \
eyedbadmin.1 \
eyedbcollimpladmin.1 \
eyedbcollimplgetdef.1 \
eyedbcollimpllist.1 \
eyedbcollimplsetdef.1 \
eyedbcollimplsimulate.1 \
eyedbcollimplstats.1 \
eyedbcollimplupdate.1 \
eyedbconsadmin.1 \
eyedbconscreate.1 \
eyedbconsdelete.1 \
eyedbconslist.1 \
eyedbdatcreate.1 \
eyedbdatdefragment.1 \
eyedbdatdelete.1 \
eyedbdatlist.1 \
eyedbdatmove.1 \
eyedbdatrename.1 \
eyedbdatresize.1 \
eyedbdbaccess.1 \
eyedbdspcreate.1 \
eyedbdspdelete.1 \
eyedbdspgetcurdat.1 \
eyedbdspgetdefault.1 \
eyedbdsplist.1 \
eyedbdsprename.1 \
eyedbdspsetcurdat.1 \
eyedbdspsetdefault.1 \
eyedbdspupdate.1 \
eyedbgetattrloca.1 \
eyedbgetcollloca.1 \
eyedbgetdefattrdsp.1 \
eyedbgetdefcolldsp.1 \
eyedbgetdefdsp.1 \
eyedbgetdefidxdsp.1 \
eyedbgetdefinstdsp.1 \
eyedbgetenv.1 \
eyedbgetidxloca.1 \
eyedbgetinstloca.1 \
eyedbgetobjloca.1 \
eyedbidxadmin.1 \
eyedbidxcreate.1 \
eyedbidxdelete.1 \
eyedbidxlist.1 \
eyedbidxsimulate.1 \
eyedbidxstats.1 \
eyedbidxupdate.1 \
eyedbjrun.1 \
eyedbloca.1 \
eyedbmvattr.1 \
eyedbmvcoll.1 \
eyedbmvidx.1 \
eyedbmvinst.1 \
eyedbmvobj.1 \
eyedbodl.1 \
eyedboql.1 \
eyedbpasswd.1 \
eyedbprotadmin.1 \
eyedbsetdefattrdsp.1 \
eyedbsetdefcolldsp.1 \
eyedbsetdefdsp.1 \
eyedbsetdefidxdsp.1 \
eyedbsetdefinstdsp.1 \
eyedbsysaccess.1 \
eyedbuseradd.1 \
eyedbuserdbaccess.1 \
eyedbuserdelete.1 \
eyedbuserlist.1 \
eyedbuserpasswd.1 

man5_MANS= \
eyedb-config.5

man7_MANS= \
eyedb.7

man8_MANS= \
eyedbctl.8 \
eyedbd.8 \
eyedbsmd.8 

XSLTPROC=@XSLTPROC@
MANPAGES_XSL_STYLESHEET=@MANPAGES_XSL_STYLESHEET_DIR@/docbook.xsl

%.1: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

%.5: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

%.7: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

%.8: $(srcdir)/%.xml
	$(XSLTPROC) $(MANPAGES_XSL_STYLESHEET) $(srcdir)/$*.xml

EXTRA_DIST= \
$(subst .1,.xml,$(man1_MANS))\
$(subst .5,.xml,$(man5_MANS))\
$(subst .7,.xml,$(man7_MANS))\
$(subst .8,.xml,$(man8_MANS))

