#!/usr/bin/env python

import zmq
import random
import sys
import time
import numpy as np

class SampleProcessor ():
    def average_IQ_power_lin_dBm (self,scaling,iq_real,iq_imag):
        scaling_lin = np.power (10,scaling/2000)
        return 10*np.log10 (np.mean (np.power (iq_real*scaling_lin,2.0) + np.power (iq_imag*scaling_lin,2.0)))

class ZmqSub ():
    def __init__ (self):
        self.ctx = zmq.Context()
        self.socket = self.ctx.socket(zmq.SUB)
        self.socket.connect("tcp://127.0.0.1:5556")
        self.socket.setsockopt_string(zmq.SUBSCRIBE,'')

    def get_msg (self):
        msg = self.socket.recv ()
        return msg

def main():
    sub = ZmqSub ()
    iq_proc = SampleProcessor ()

    while (True):
        msg = sub.get_msg ()
        # print (msg)
        print (len(msg))
        nb_of_channels = np.frombuffer(msg,dtype='uint32',count=1,offset=0)[0]
        print('Number of channels: {}'.format (nb_of_channels))
        nb_of_samples = np.frombuffer(msg,dtype='uint32',count=1,offset=4)[0]
        print('Number of samples: {}'.format (nb_of_samples))

        sample_rate = np.frombuffer(msg,dtype='float64',count=1,offset=8)[0]
        print('Sample rate: {:.2f}'.format (sample_rate))

        iq_start = np.frombuffer(msg,dtype='uint64',count=1,offset=16)[0]
        print('IQ start time: {}'.format (iq_start))

        offset = 24
        scaling =  np.frombuffer(msg,dtype='int16',count=nb_of_channels,offset=offset)
        print('{} scaling: {}'.format (offset,scaling))

        offset = offset+(nb_of_channels*2)
        overflow =  np.frombuffer(msg,dtype='uint32',count=nb_of_channels,offset=offset)
        print('{} overflow: {}'.format (offset,overflow))

        # Contains all channels
        offset = offset+(nb_of_channels*4)
        real = np.frombuffer(msg,dtype='float64',count=nb_of_channels*nb_of_samples,offset=offset)

        offset = offset+(nb_of_channels*nb_of_samples*8)
        imag = np.frombuffer(msg,dtype='float64',count=nb_of_channels*nb_of_samples,offset=offset)

        print('{} real[0], imag[0]: {} {}'.format (offset,real[0],imag[0]))
        print ('IQ power first sample: {:.2f}'.format (iq_proc.average_IQ_power_lin_dBm (scaling[0],real[0],imag[0])))
        print ('IQ power: {:.2f}'.format (iq_proc.average_IQ_power_lin_dBm (scaling[0],real[:nb_of_samples],imag[:nb_of_samples])))
        if nb_of_channels > 1:
            print ('real[0],imag[0]: {} {}'.format (real[nb_of_samples],imag[nb_of_samples]))
            print ('IQ power first sample: {:.2f}'.format (iq_proc.average_IQ_power_lin_dBm (scaling[1],real[nb_of_samples],imag[nb_of_samples])))
            print ('IQ power: {:.2f}'.format (iq_proc.average_IQ_power_lin_dBm (scaling[1],real[nb_of_samples:],imag[nb_of_samples:])))


        #imag = np.frombuffer(msg,dtype='float64',count=nb_of_channels*nb_of_samples,offset=40+(nb_of_channels*nb_of_samples*8))
        #print (real[0],imag[0])

if __name__ == '__main__':
    main ()
