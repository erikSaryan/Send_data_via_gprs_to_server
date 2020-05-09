#!/usr/bin/python
import socket

class Server():
    def __init__(self, ip, port):
        self.host = ip
        self.port = port
        self.main()

    def connect_socket(self):
        serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serversocket.bind((self.host, self.port))
        serversocket.listen(5)
        return serversocket

    def main(self):
        try:
            srv = self.connect_socket()
            print("Server started")
        except Exception as x:
            print(x)
        while True:
            (clientsocket, address) = srv.accept()
            data = str(clientsocket.recv(1024).decode())
            print(data)
            clientsocket.close()
        srv.close()

# Local server port and ip to which GPRS data will be sent
server = Server("192.168.10.8", 8071)