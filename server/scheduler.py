import os
import sys

sys.path.append(os.path.join(os.path.abspath(os.path.dirname(__file__)), '..'))
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "settings")

import socket  # Import socket module
import time
from django.conf import settings

from django.db import models
from .PinPRIMES.pp_main_app.models import *
import json

while True:
    time.sleep(10)
    task = Task.objects.all().order_by("order")[0]

    s = socket.socket()
    host = task.master_host
    port = 12345

    try:
        s.connect((host, port))
        s.send(bytes(r'{}'.format(str(task.number)), 'ASCII'))
        print(s.recv(1024))
    except:
        s.close()
    s.close()  # Close the socket when done
