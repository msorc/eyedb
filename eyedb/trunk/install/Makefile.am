SUBDIRS=etc 

install-data-local:
#	EV : 17/02/07: disconnected the following line:
#	$(mkinstalldirs) $(DESTDIR)$(localstatedir)/$(PACKAGE)/db $(DESTDIR)$(localstatedir)/$(PACKAGE)/pipes $(DESTDIR)$(localstatedir)/$(PACKAGE)/tmp
	$(mkinstalldirs) $(DESTDIR)$(localstatedir)/lib/$(PACKAGE)/db $(DESTDIR)$(localstatedir)/lib/$(PACKAGE)/pipes $(DESTDIR)$(localstatedir)/lib/$(PACKAGE)/tmp
# 	EV : 26/02/07: patch right access on tmp directory: not the good way to do it -> FD must correct that	
	chmod a+w $(DESTDIR)$(localstatedir)/lib/$(PACKAGE)/tmp
