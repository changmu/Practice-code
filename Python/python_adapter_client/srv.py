#!/usr/bin/python
import socket
import struct
import time
import signal
import sys

pkt_recv = 0

def sighandler(sig, frame):
    if (sig == signal.SIGINT):
        print "recv %d pkts" % (pkt_recv)
        sys.exit(0)

signal.signal(signal.SIGINT, sighandler)

s = socket.socket()
s.connect(("10.0.0.163", 7070))

pkt_length = 14
pkt_type = 234
pkt_recv_user_id = 1
pkt_send_user_id = 2

pkt = struct.pack("<IHII", pkt_length, pkt_type, pkt_recv_user_id, pkt_send_user_id)

s.send(pkt)
time.sleep(1)

while (1):
    ret = s.recv(1000)
    pkt_recv += 1
