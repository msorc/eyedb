noinst_PYTHON=\
common.py \
user.py

#EDIT = sed \
#        -e 's,@bindir\@,$(bindir),g' 
#
#%.py: Makefile %.py.in
#	rm -f $*.py $*.py.tmp
#	$(EDIT) $(srcdir)/$*.py.in >$*.py.tmp
#	mv $*.py.tmp $*.py
#	chmod +x $*.py

TESTS_ENVIRONMENT= PATH=$PATH:$(bindir) PYTHONPATH=$(srcdir):$PYTHONPATH $(PYTHON) 
TESTS=\
user.py
