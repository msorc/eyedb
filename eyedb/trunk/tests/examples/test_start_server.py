import sys
import os
from eyedb.test.server import server_start

status = server_start( sbindir = os.environ['sbindir'])
sys.exit(status)
    
