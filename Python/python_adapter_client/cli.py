#!/usr/bin/python
import socket
import struct
import time

s = socket.socket()
s.connect(("10.0.0.92", 7070))

pkt_length = 14
pkt_type = 234
pkt_recv_user_id = 2
pkt_send_user_id = 1

pkt = struct.pack("<IHII", pkt_length, pkt_type, pkt_recv_user_id, pkt_send_user_id)

s.send(pkt)
time.sleep(1)

pkt_length = 1000
pkt_type = 200
pkt_recv_user_id = 2
pkt_send_user_id = 1
data = "abcdefg"
pkt = struct.pack("<IHII986s", pkt_length, pkt_type, pkt_recv_user_id, pkt_send_user_id, data)

pkt_all_cnt = 1000000
pkt_ok_cnt = 0
cnt = 0

for i in range(0, pkt_all_cnt):
    # if cnt == 100:
        # cnt = 0
        # time.sleep(0.001)

    ret = s.send(pkt)
    if ret == len(pkt):
        pkt_ok_cnt += 1

    cnt += 1

print "ok/all %d/%d" % (pkt_ok_cnt, pkt_all_cnt)
