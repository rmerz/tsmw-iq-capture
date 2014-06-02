#!/usr/bin/env python

import zmq
import random
import sys
import time
import numpy as np


# ctx = zmq.Context.instance()
# s = ctx.socket(zmq.REQ)
# s.connect("tcp://127.0.0.1:5556")
# s.send_string('READY')
# msg = s.recv_string()
# print(msg)

ctx = zmq.Context()
s = ctx.socket(zmq.SUB)
s.connect("tcp://127.0.0.1:5556")
s.setsockopt_string(zmq.SUBSCRIBE,'')
#msg = s.recv_string()
msg = s.recv()
print (msg)
print (len(msg))
print(np.frombuffer(msg,dtype='uint32',count=1,offset=0))
print(np.frombuffer(msg,dtype='float64',count=1,offset=4))
print(np.frombuffer(msg,dtype='float64',count=4,offset=12))
