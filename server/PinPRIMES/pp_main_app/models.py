from enum import Enum

from django.db import models
from django.contrib.auth.models import User
import json
from datetime import datetime


# Create your models here.
class AlgorithmType:
    aks = 0
    miller_rabin = 1


algorithm_type_map = {'aks': 0, 'mr': 1}


class TaskState:
    pending = 0
    running = 1
    stopping = 2
    stopped = 3
    finished = 4


class Task(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    algorithm = models.IntegerField()
    state = models.IntegerField()
    number = models.BigIntegerField()
    order = models.IntegerField()
    master_host = models.CharField(max_length=128)
    result = models.IntegerField(default=0)
    progress = models.IntegerField(default=0)
    time_begin = models.DateTimeField(default=datetime.now())
    time_end = models.DateTimeField(default=datetime(1970, 1, 1, 0, 0))

    def to_json(self):
        return json.dumps([{
            'object_type': 'task',
            'user': self.user.get_username(),
            'algorithm': self.algorithm,
            'state': self.state,
            'number': self.number,
            'order': self.order,
            'master_host': self.master_host,
            'result': self.result,
            'progress': self.progress,
            'time_begin': self.time_begin.isoformat(),
            'time_end': self.time_end.isoformat() or datetime(1970, 1, 1, 0, 0)
        }])

    def from_json(self, json_object):
        self.user = User.objects.get_by_natural_key(json_object['user'])
        self.algorithm = json_object['algorithm']
        self.state = json_object['state']
        self.number = json_object['number']
        self.order = json_object['order']
        self.master_host = json_object['master_host']
        self.result = json_object['result']
        self.progress = json_object['progress']
        self.time_begin = json_object['time_begin']
        self.time_end = json_object['time_end']
