TESTS_ENVIRONMENT= $(SHELL) 

TESTS=\
test_odl_remove_schema.sh

DATABASE=bugs_odl_remove_schema
SCHEMA=$(srcdir)/schema.odl

init:
	-$(bindir)/eyedbadmin database delete $(DATABASE)
	$(bindir)/eyedbadmin database create $(DATABASE)
	$(bindir)/eyedbodl --update --database=$(DATABASE) $(SCHEMA)
.PHONY: init

# Code snippet from autoconf documentation:
# http://www.gnu.org/software/autoconf/manual/autoconf.html#Installation-Directory-Variables
EDIT= sed \
        -e 's,@bindir[@],$(bindir),g' \
        -e 's,@exec_prefix[@],$(exec_prefix),g' \
        -e 's,@prefix[@],$(prefix),g' 

%.sh: Makefile $(srcdir)/%.sh.in
	rm -f $@ $@.tmp
	$(EDIT) '$(srcdir)/$@.in' >$@.tmp
	mv $@.tmp $@

EXTRA_DIST=test_odl_remove_schema.sh.in schema.odl


