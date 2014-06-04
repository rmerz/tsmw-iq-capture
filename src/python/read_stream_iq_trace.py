#!/usr/bin/env python

import argparse
import numpy as np
from scipy import signal
from matplotlib import pylab as plt

def setup_args():
    parser = argparse.ArgumentParser(description='Extract data from binary file obtained with extract_iq.')
    parser.add_argument('filepath', type=str, help='File to open.')
    parser.add_argument('--header_length', type=int, default=512, help='Length of the text header (default is 512)')
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
    return np.fromfile (data,np.dtype ('float64'), count = n)

def decode_fe_freq (data):
    return np.fromfile (data,np.dtype ('uint64'), count = 2)

def main (args):
    print (args.filepath)

    f = open (args.filepath,'rb')

    # This assumes you already know the length of the header
    header = decode_header (f.read (args.header_length))
    print ('Header: ',header)

    FE = decode_fe_freq (f)
    print ('Front-end frequencies: {}'.format (FE))
    number_of_channels = decode_uint32 (f,2)
    print ('Number of channels: {}'.format (number_of_channels))
    block_size = decode_uint32 (f)
    print ('Block size: {}'.format (block_size))
    while (True):
        # This assumes you know what to expect
        start_time_iq = decode_uint64 (f)
        if len (start_time_iq) == 0:
            break
        print (start_time_iq)
        sample_rate = decode_float64 (f)
        print (sample_rate)
        scaling = decode_int16 (f,n=1)  # Scaling
        print (scaling)
        real = decode_float64 (f,n=block_size) # Real
        imag = decode_float64 (f,n=block_size) # Real
        print (real[0])
        print (imag[0])

        scaling_lin = np.power (10,scaling/2000)
        print (10*np.log10(np.mean (np.power (real*scaling_lin,2.0) + np.power (imag*scaling_lin,2.0))))

    # Plot the last block
    real_scaled = real*scaling_lin
    imag_scaled = imag*scaling_lin

    # f, Pxx_den = signal.periodogram(real_scaled+np.complex(0,1)*imag_scaled, nfft=2048)
    f, Pxx_den = signal.welch(real_scaled+np.complex(0,1)*imag_scaled,
                              fs = sample_rate,
                              #scaling='density',
                              scaling='spectrum',
                              nperseg=4096)

    plt.ion ()
    plt.plot (f, 10*np.log10 (Pxx_den))
    # plt.ylim ([1e-11, 1e-5])
    plt.grid (True)
    plt.tight_layout ()
    input ('Press any key.')

if __name__ == '__main__':
    main (setup_args ())
