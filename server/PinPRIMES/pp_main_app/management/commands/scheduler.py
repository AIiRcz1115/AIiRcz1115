import socket  # Import socket module
import time
from django.core.management.base import BaseCommand, CommandError
from pp_main_app.models import *
import json
from datetime import datetime
from itertools import chain


class Command(BaseCommand):
    def add_arguments(self, parser):
        return

    def send_task(self, task):
        print('Got task')
        s = socket.socket()
        host = task.master_host
        port = 12345
        s.connect((host, port))
        s.send(bytes(r'{}'.format(task.to_json()), 'ASCII'))
        s_input = s.recv(1024).decode('ASCII')
        print(s_input)
        task.from_json(json.loads(s_input))
        task.save()
        print('Task state changed')
        s.close()  # Close the socket when done

    def handle(self, *args, **options):
        print('Started')
        while True:
            if Task.objects.filter(state=TaskState.stopping).exists():
                try:
                    self.send_task(Task.objects.filter(state=TaskState.stopping)[0])
                except:
                    pass
            if Task.objects.filter(state=TaskState.pending).exists():
                try:
                    task = Task.objects.filter(state=TaskState.pending).order_by("order")[0]
                except:
                    pass
                if not Task.objects.filter(master_host=task.master_host, state=TaskState.running).exists():
                    self.send_task(task)

            tasks_r = Task.objects.filter(state=TaskState.running)
            tasks_s = Task.objects.filter(state=TaskState.stopping)
            tasks = list(chain(tasks_r, tasks_s))

            for task in tasks:
                s = socket.socket()
                host = task.master_host
                port = 12345
                s.connect((host, port))
                s.send(bytes(r'{}'.format(task.to_json()), 'ASCII'))
                s_input = s.recv(1024).decode('ASCII')
                s.close()
                print(s_input)
                task.from_json(json.loads(s_input))
                if task.state == TaskState.finished or task.state == TaskState.stopped:
                    task.time_end = datetime.now()
                task.save()
                print('Task updated')

            time.sleep(10)
