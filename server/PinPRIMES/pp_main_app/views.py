from django.http import HttpResponse
from django.template import RequestContext
from django.shortcuts import render, render_to_response, redirect
from django.views.decorators.csrf import csrf_protect
import socket  # Import socket module
import json
from django.shortcuts import get_object_or_404
from .models import *


def index(request):
    context = dict()
    context['tasks'] = Task.objects.all
    context['user'] = request.user
    context['message'] = ''
    return render(request, 'main.html', context)


@csrf_protect
def run_primality_test(request):
    if request.POST:
        task = Task()
        task.algorithm = algorithm_type_map[request.POST['algorithm']]
        task.number = request.POST['number_to_test']
        task.state = TaskState.pending
        task.user = request.user
        task.master_host = request.POST['master_address']
        task.result = -1
        task.processes = request.POST['processes']
        try:
            task.order = Task.objects.all().order_by("-order")[0].order + 1
        except:
            task.order = 0
        task.save()
    context = dict()
    context['tasks'] = Task.objects.all()
    context['user'] = request.user
    context['message'] = ''
    return render_to_response('main.html', context, RequestContext(request))


def stop_task(request, task_id):
    task = get_object_or_404(Task, pk=task_id)
    if request.user.is_authenticated() and (request.user == task.user or request.user.is_superuser):
        if task.state == TaskState.running:
            task.state = TaskState.stopping
            message = 'Task is stopping.'
        elif task.state == TaskState.pending:
            task.state = TaskState.stopped
            task.time_end = datetime.now()
            message = 'Task stopped.'
        else:
            message = 'You cannot stop this task, it is already stopped or finished.'
        task.save()
    else:
        message = 'You cannot stop this task, you are not a owner nor superuser.'
    context = dict()
    context['tasks'] = Task.objects.all()
    context['user'] = request.user
    context['message'] = message
    return render_to_response('main.html', context, RequestContext(request))