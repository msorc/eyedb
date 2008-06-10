import os
from eyedb.test.command import run_simple_command

cpp_test = "%s/examples/C++Binding/schema-oriented/admin/admin" % (os.environ['top_builddir'],)
run_simple_command( cpp_test)

