import socket

HOST = '192.168.0.11'
PORT = 8080

s= socket.socket(socket.AF_INET,  socket.SOCK_STREAM)

s.connect((HOST,PORT))

while True:
    #command = input('Enter your command: ')
    #s.send(command.encode('utf-8'))
    reply = s.recv(1024)

    if reply == 'Terminate':
        break

    print(reply.decode('utf-8'))


