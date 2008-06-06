import os
from eyedb.test.command import run_simple_command

# run the GettingStarted Java part
java_test = "%s -cp %s/examples/GettingStarted:%s/eyedb/java/eyedb.jar PersonTest --user=%s person_g" % (os.environ['JAVA'], os.environ['top_builddir'],os.environ['libdir'], os.environ['USER'])
print java_test
run_simple_command( java_test)


