TESTS_ENVIRONMENT= bindir=$(bindir) PYTHONPATH=$(top_srcdir)/tests/lib/python:$PYTHONPATH $(PYTHON) 

TESTS=\
test_odl_remove_schema.py

DATABASE=bugs_odl_remove_schema
SCHEMA=$(top_srcdir)/examples/GettingStarted/schema.odl

init:
	$(bindir)/eyedbadmin database create $(DATABASE)
	$(bindir)/eyedbodl --update --database=$(DATABASE) $(SCHEMA)
.PHONY: init

noinst_PYTHON= $(TESTS)
