from django.http import HttpResponse
from django.template import RequestContext
from django.shortcuts import render, render_to_response
from django.views.decorators.csrf import csrf_protect
import socket               # Import socket module

def index(request):
    return render(request, 'main.html', {})

@csrf_protect
def run_primality_test(request):
    if request.POST:
        master_address = request.POST['master_address']
        number = request.POST['number_to_test']

        s = socket.socket()         # Create a socket object
        host = master_address       # Get local machine name
        port = 12345                # Reserve a port for your service.

        try:
            s.connect((host, port))
            s.send(bytes(r'{}'.format(str(number)), 'ASCII'))
            print(s.recv(1024))
        except:
            s.close()
        s.close()                     # Close the socket when done
    return render_to_response('main.html', RequestContext(request))
