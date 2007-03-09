man1_MANS= \
eyedbgetenv.1 \
eyedbadmin.1 \
eyedbdbcreate.1 \
eyedbdbdelete.1 \
eyedbdblist.1 \
eyedbdbcopy.1 \
eyedbdbmove.1 \
eyedbdbrename.1 \
eyedbdbexport.1 \
eyedbdbimport.1 \
eyedbdbmcreate.1 \
eyedbdatcreate.1 \
eyedbdatdelete.1 \
eyedbdatlist.1 \
eyedbdatmove.1 \
eyedbdatrename.1 \
eyedbdatresize.1 \
eyedbdatdefragment.1 \
eyedbdspcreate.1 \
eyedbdspdelete.1 \
eyedbdsplist.1 \
eyedbdsprename.1 \
eyedbdspupdate.1 \
eyedbdspgetcurdat.1 \
eyedbdspgetdefault.1 \
eyedbdspsetcurdat.1 \
eyedbdspsetdefault.1 \
eyedbodl.1 \
eyedboql.1 \
eyedbsysaccess.1 \
eyedbuseradd.1 \
eyedbuserdbaccess.1 \
eyedbuserdelete.1 \
eyedbuserlist.1 \
eyedbuserpasswd.1 \
eyedbpasswd.1 \
eyedbdbaccess.1 \
eyedbprotadmin.1 \
eyedbidxadmin.1 \
eyedbidxcreate.1 \
eyedbidxdelete.1 \
eyedbidxlist.1 \
eyedbidxstats.1 \
eyedbidxupdate.1 \
eyedbidxsimulate.1 \
eyedbjrun.1 \
eyedbloca.1 \
eyedbgetattrloca.1 \
eyedbgetcollloca.1 \
eyedbgetdefattrdsp.1 \
eyedbgetdefcolldsp.1 \
eyedbgetdefdsp.1 \
eyedbgetdefidxdsp.1 \
eyedbgetdefinstdsp.1 \
eyedbgetidxloca.1 \
eyedbgetinstloca.1 \
eyedbgetobjloca.1 \
eyedbsetdefattrdsp.1 \
eyedbsetdefcolldsp.1 \
eyedbsetdefdsp.1 \
eyedbsetdefidxdsp.1 \
eyedbsetdefinstdsp.1 \
eyedbmvattr.1 \
eyedbmvcoll.1 \
eyedbmvidx.1 \
eyedbmvinst.1 \
eyedbmvobj.1 \
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
eyedbconslist.1

man5_MANS= \
eyedb-config.5

man7_MANS= \
eyedb-options.7 \
eyedb.7

man8_MANS= \
eyedbd.8 \
eyedbsmd.8 \
eyedbctl.8

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
eyedb-options.xml \
eyedb.xml \
eyedb-config.xml \
eyedbd.xml \
eyedbsmd.xml \
eyedbctl.xml \
eyedbgetenv.xml \
eyedbadmin.xml \
eyedbdbcreate.xml \
eyedbdbdelete.xml \
eyedbdblist.xml \
eyedbdbcopy.xml \
eyedbdbmove.xml \
eyedbdbrename.xml \
eyedbdbexport.xml \
eyedbdbimport.xml \
eyedbdbmcreate.xml \
eyedbdatcreate.xml \
eyedbdatdelete.xml \
eyedbdatlist.xml \
eyedbdatmove.xml \
eyedbdatrename.xml \
eyedbdatresize.xml \
eyedbdatdefragment.xml \
eyedbdspcreate.xml \
eyedbdspdelete.xml \
eyedbdsplist.xml \
eyedbdsprename.xml \
eyedbdspupdate.xml \
eyedbdspgetcurdat.xml \
eyedbdspgetdefault.xml \
eyedbdspsetcurdat.xml \
eyedbdspsetdefault.xml \
eyedbodl.xml \
eyedboql.xml \
eyedbsysaccess.xml \
eyedbuseradd.xml \
eyedbuserdbaccess.xml \
eyedbuserdelete.xml \
eyedbuserlist.xml \
eyedbuserpasswd.xml \
eyedbpasswd.xml \
eyedbdbaccess.xml \
eyedbprotadmin.xml \
eyedbidxadmin.xml \
eyedbidxcreate.xml \
eyedbidxdelete.xml \
eyedbidxlist.xml \
eyedbidxstats.xml \
eyedbidxupdate.xml \
eyedbidxsimulate.xml \
eyedbjrun.xml \
eyedbloca.xml \
eyedbgetattrloca.xml \
eyedbgetcollloca.xml \
eyedbgetdefattrdsp.xml \
eyedbgetdefcolldsp.xml \
eyedbgetdefdsp.xml \
eyedbgetdefidxdsp.xml \
eyedbgetdefinstdsp.xml \
eyedbgetidxloca.xml \
eyedbgetinstloca.xml \
eyedbgetobjloca.xml \
eyedbsetdefattrdsp.xml \
eyedbsetdefcolldsp.xml \
eyedbsetdefdsp.xml \
eyedbsetdefidxdsp.xml \
eyedbsetdefinstdsp.xml \
eyedbmvattr.xml \
eyedbmvcoll.xml \
eyedbmvidx.xml \
eyedbmvinst.xml \
eyedbmvobj.xml \
eyedbcollimpladmin.xml \
eyedbcollimplgetdef.xml \
eyedbcollimpllist.xml \
eyedbcollimplsetdef.xml \
eyedbcollimplsimulate.xml \
eyedbcollimplstats.xml \
eyedbcollimplupdate.xml \
eyedbconsadmin.xml \
eyedbconscreate.xml \
eyedbconsdelete.xml \
eyedbconslist.xml
