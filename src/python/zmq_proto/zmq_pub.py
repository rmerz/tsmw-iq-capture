#!/usr/bin/env python

import zmq
import random
import sys
import time


# ctx = zmq.Context.instance()
# s = ctx.socket(zmq.REP)
# s.bind("tcp://127.0.0.1:5556")
# print("Waiting for subscriber to connect...")
# s.recv()
# print(" Done.")
# s.send_string('GO')

ctx = zmq.Context()
s = ctx.socket(zmq.PUB)
s.bind("tcp://127.0.0.1:5556")
while (True):
    print("broadcast")
    s.send_string('GO')
    time.sleep (5)
