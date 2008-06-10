import os
from eyedb.test.command import run_simple_command

cpp_test = [ "%s/examples/C++Binding/generic/basic" % (os.environ['top_builddir'],),
             'person_c',
             'select Person']
run_simple_command( cpp_test)

