#!/usr/bin/python

# Usage example
#   python vim-client.py "ex" "e alloc.h"
#   python vim-client.py "redraw" ""
#
# Connect in vim
#   :let handle = ch_open('localhost:8765')

import socket
import sys
import json

SEND_TO_VIM = "send_to_vim"
HOST, PORT = "localhost", 8765

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to server and send data
    sock.connect((HOST, PORT))

    json_data = json.dumps([SEND_TO_VIM,sys.argv[1],sys.argv[2]])
    sock.sendall(json_data.encode('utf-8'))
finally:
    sock.close()

