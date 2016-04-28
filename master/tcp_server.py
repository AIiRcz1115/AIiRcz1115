import socket  # Import socket module
import subprocess

s = socket.socket()  # Create a socket object
host = ''
port = 12345  # Reserve a port for your service.
s.bind((host, port))  # Bind to the port

s.listen(5)  # Now wait for client connection.
while True:
    c, addr = s.accept()  # Establish connection with client.
    try:
        print('Got connection from', addr)
        s_input = c.recv(1024)
        #subprocess.Popen('mpirun -np 6 -hosts master,slave1,slave2 ./aks {}'.format(str(s_input)), shell=True)
        subprocess.call(r'mpirun -np 6 ../MPI-aks/aks {}'.format(str(s_input)), shell=True)
    except:
        c.close()
    c.close()  # Close the connection
