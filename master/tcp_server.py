import socket  # Import socket module
import subprocess
import json
import _thread
import time
import os
import signal
import shlex


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


task_object = None
task_process = None


def run_command(command):
    global task_object
    global task_process
    task_process = subprocess.Popen(command, stderr=subprocess.PIPE, preexec_fn=os.setsid, shell=False)
    while True:
        output = task_process.stderr.readline().decode()
        # if output == '' and task_process.poll() is not None:
        if task_process.poll() is not None:
            break
        if output:
            line = output.strip()
            line_split = line.split(' ')
            if len(line_split) == 2:
                task_object['result'] = int(line_split[1])
                break
            elif len(line_split) == 1:
                task_object['progress'] = int(line)
            print('Progress:', line)

    rc = task_process.poll()
    task_object['state'] = TaskState.finished
    print('Task finished')
    return rc


def handle_connection(connection, addr):
    global task_object
    global task_process
    print('Got connection from', addr)
    s_input = connection.recv(1024).decode('ASCII')
    print('Content:\n{}'.format(s_input))
    json_input = json.loads(s_input)[0]
    if json_input['state'] == TaskState.pending:
        task_object = json_input
        cmd = None
        if task_object['algorithm'] == AlgorithmType.miller_rabin:
            cmd = ['mpirun', '-np', '4', '-hosts', 'localhost',
                   '../MPI-program/prime', 'mr', '{}'.format(task_object['number']), '100000']
        elif task_object['algorithm'] == AlgorithmType.aks:
            cmd = ['mpirun', '-np', '4', '-hosts', 'localhost',
                   '../MPI-program/prime', 'aks', '{}'.format(task_object['number'])]

        _thread.start_new_thread(run_command, (cmd,))

        task_object['state'] = TaskState.running
        s_output = json.dumps(task_object)
        connection.send(bytes(r'{}'.format(s_output), 'ASCII'))
        connection.close()
    elif json_input['state'] == TaskState.running:
        s_output = json.dumps(task_object)
        print('Sending:', s_output)
        connection.send(bytes(r'{}'.format(s_output), 'ASCII'))
        connection.close()
    elif json_input['state'] == TaskState.stopping:
        os.killpg(os.getpgid(task_process.pid), signal.SIGTERM)  # Send the signal to all the process groups
        task_object = json_input
        task_object['state'] = TaskState.stopped
        s_output = json.dumps(task_object)
        connection.send(bytes(r'{}'.format(s_output), 'ASCII'))
        connection.close()


s = socket.socket()  # Create a socket object
host = ''
port = 12345  # Reserve a port for your service.
s.bind((host, port))  # Bind to the port

s.listen(5)  # Now wait for client connection.

while True:
    c, addr = s.accept()
    handle_connection(c, addr)
