#!/usr/bin/env python

import argparse
import numpy as np
from scipy import signal
from matplotlib import pylab as plt

def setup_args():
    parser = argparse.ArgumentParser(description='Extract data from binary file obtained with get_iq.')
    parser.add_argument('filepath', type=str, help='File to open.')
    args   = parser.parse_args()
    return args

def decode_header (header):
    eos = 0
    for b in header:
        if b == 0:
            break
        eos = eos +1
    return header[:eos].decode ()

def decode_int16 (data,n=1):
    return np.fromfile (data,np.dtype ('int16'), count = n)

def decode_uint32 (data,n=1):
    return np.fromfile (data,np.dtype ('uint32'), count = n)

def decode_uint64 (data,n=1):
    return np.fromfile (data,np.dtype ('uint64'), count = n)

def decode_float64 (data,n=1):
    return np.fromfile (data,np.dtype ('d8'), count = n)

def decode_fe_freq (data):
    return np.fromfile (data,np.dtype ('uint64'), count = 2)

def average_iq_power (real_lin,imag_lin):
    return 10*np.log10(np.mean (np.power (real_lin,2.0) + np.power (imag_lin[0],2.0)))

def main (args):
    print (args.filepath)

    f = open (args.filepath,'rb')

    # This assumes you already know the length of the header
    header = decode_header (f.read (512))
    print (header)
    # This assumes you know what to expect
    fe1,fe2 = decode_fe_freq (f)
    print (fe1)
    print (fe2)
    number_of_channels = decode_uint32 (f)
    print (number_of_channels)
    block_size = decode_uint32 (f)
    print (block_size)
    while (True):
        start_time_iq = decode_uint64 (f)
        if len (start_time_iq) == 0:
            break
        print (start_time_iq)  # StartTimeIQ
        sample_rate = decode_float64 (f)
        print ('Sampling rate:', sample_rate)  # Fsample
        scaling = decode_int16 (f,n=number_of_channels)  # Scaling
        scaling_lin = np.power (10,scaling/2000)
        print (scaling)
        real = np.fromfile (f,np.dtype ('double'), count = number_of_channels*block_size)
        imag = np.fromfile (f,np.dtype ('double'), count = number_of_channels*block_size)
        # print (real[0])
        # print (imag[0])

        # Assuming two channels and scaling is in dBm, hence scaling lin is in mV
        real_lin_ch1 = real[:block_size]*scaling_lin[0]
        imag_lin_ch1 = imag[:block_size]*scaling_lin[0]
        print (average_iq_power (real_lin_ch1,imag_lin_ch1))
        real_lin_ch2 = real[block_size:]*scaling_lin[1]
        imag_lin_ch2 = imag[block_size:]*scaling_lin[1]
        print (average_iq_power (real_lin_ch2,imag_lin_ch2))

    # Display last block. And see 4.27 in
    # http://www.ni.com/pdf/manuals/370192c.pdf for why spectrum
    # versus density: PSD is PS / (1/sampling_rate * noise_bandwidth)    
    f, Pxx_den = signal.welch(real_lin_ch1+np.complex(0,1)*imag_lin_ch1,
                              fs = sample_rate,
                              scaling='spectrum',
                              nperseg=512)

    plt.ion ()
    plt.plot (f, 10*np.log10 (Pxx_den))
    # plt.ylim ([1e-11, 1e-5])
    plt.grid (True)
    plt.tight_layout ()
    input ('Press any key.')

        
    

if __name__ == '__main__':
    main (setup_args ())
