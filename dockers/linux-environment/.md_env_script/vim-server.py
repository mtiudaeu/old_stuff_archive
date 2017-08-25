#!/usr/bin/python
#
# This is a modified version of a demo socket server provide
# in vim 8.0.
#
# Server that will accept connections from a Vim channel.
# Run this server and then in Vim you can open the channel:
#  :let handle = ch_open('localhost:8765')
#
# Then Vim can send requests to the server:
#  :let response = ch_sendexpr(handle, 'hello!')
#
# And you can control Vim by typing a JSON message here, e.g.:
#   ["ex","echo 'hi there'"]
#
# There is no prompt, just type a line and press Enter.
# To exit cleanly type "quit<Enter>".
#
# See ":help channel-demo" in Vim.
#
# This requires Python 2.6 or later.

from __future__ import print_function
import json
import socket
import sys
import threading
import getopt
import time

try:
    # Python 3
    import socketserver
except ImportError:
    # Python 2
    import SocketServer as socketserver

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

SEND_TO_VIM = "send_to_vim"
HOST, PORT = "localhost", 8765
socket_array = [];

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        print("=== socket opened ===")
        global socket_array
        socket_array.append(self.request)
        while True:
            try:
                data = self.request.recv(4096).decode('utf-8')
            except socket.error:
                print("=== socket error ===")
                break
            except IOError:
                print("=== socket closed ===")
                break
            if data == '':
                print("=== socket closed ===")
                break
            print("received: {0}".format(data))
            try:
                decoded = json.loads(data)
            except ValueError:
                print("json decoding failed")
                decoded = [-1, '']

            if decoded[0] == SEND_TO_VIM:
                # Sending to vim
                encoded = json.dumps([decoded[1],decoded[2]])
                print("sending {0}".format(encoded))
                for s in socket_array:
                    s.sendall(encoded.encode('utf-8'))
            elif decoded[0] >= 0:
                # Sent from vim
                # Send a response if the sequence number is positive.
                # Negative numbers are used for "eval" responses.
                if decoded[1] == 'hello!':
                    response = "got it"
                else:
                    response = "what?"
                encoded = json.dumps([decoded[0], response])
                print("sending {0}".format(encoded))
                self.request.sendall(encoded.encode('utf-8'))

        socket_array.remove(self.request)

def run_vim_server(debug):
    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)

    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()
    print("Server loop running in thread: ", server_thread.name)

    print("Listening on port {0}".format(PORT))
    while True:
        if debug is True:
            typed = sys.stdin.readline()
            if "quit" in typed:
                print("Goodbye!")
                break
            print("sending {0}".format(typed))
            for s in socket_array:
                s.sendall(typed.encode('utf-8'))
        else:
            time.sleep(10)

    server.shutdown()
    server.server_close()

def args_print_usage():
    print("Usage : python vim-server.py")
    print("-h : help")
    print("--debug : Enable debugging and manually send command to vim")

def args_read(argv):
   debug = False;
   try:
      opts, args = getopt.getopt(argv,"h",["debug"])
   except getopt.GetoptError:
      args_print_usage()
      sys.exit(1)
   for opt, arg in opts:
      if opt == '-h':
         args_print_usage()
         sys.exit()
      elif opt in ("--debug"):
         debug = True

   return debug

if __name__ == "__main__":
   debug = args_read(sys.argv[1:]) 
   run_vim_server(debug) 

# TODO List
# -In args_read : Add -p option to chose port.
# -Change print for log into file.



