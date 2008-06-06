import sys
import os
from eyedb.test.server import server_stop

status = server_stop( sbindir = os.environ['sbindir'])
sys.exit(status)

