import socket  # Import socket module
import time
from django.core.management.base import BaseCommand, CommandError
from pp_main_app.models import *
import json
from datetime import datetime


class Command(BaseCommand):
    def add_arguments(self, parser):
        return

    def handle(self, *args, **options):
        print('Started')
        while True:
            # try:
            #     tasks = Task.objects.filter(state=TaskState.running).order_by("order")
            #     for task in tasks:
            #         s.connect((host, port))
            #         s.send(bytes(r'{}'.format(task.to_json()), 'ASCII'))
            #         s_input = s.recv(1024)
            #         print(s_input)
            #         task.from_json(json.loads(s_input)[0])
            #         task.save()
            #         print('Task updated')
            # except:
            #     pass
            tasks = Task.objects.filter(state=TaskState.running).order_by("order")
            for task in tasks:
                s = socket.socket()
                host = task.master_host
                port = 12345
                s.connect((host, port))
                s.send(bytes(r'{}'.format(task.to_json()), 'ASCII'))
                s_input = s.recv(1024).decode('ASCII')
                print(s_input)
                task.from_json(json.loads(s_input))
                if task.state == TaskState.finished or task.state == TaskState.stopped:
                    task.time_end = datetime.now().isoformat()
                task.save()
                print('Task updated')

            if Task.objects.filter(state=TaskState.pending).exists():
                task = Task.objects.filter(state=TaskState.pending).order_by("order")[0]
                if not Task.objects.filter(master_host=task.master_host, state=TaskState.running).exists():
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

            time.sleep(10)
