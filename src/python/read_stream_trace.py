#!/usr/bin/env python

import argparse
import numpy as np
import struct

def setup_args():
    parser = argparse.ArgumentParser(description='Extract data from binary file obtained with extract_iq.')
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
    return np.fromfile (data,np.dtype ('float64'), count = n)

def decode_fe_freq (data):
    return np.fromfile (data,np.dtype ('uint64'), count = 2)

def main (args):
    print (args.filepath)

    f = open (args.filepath,'rb')

    # This assumes you already know the length of the header
    header = decode_header (f.read (256))
    print (header)

    while (True):
        # This assumes you know what to expect
        block_size = decode_uint32 (f)
        if len (block_size) == 0:
            break
        print (block_size)
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


        
    

if __name__ == '__main__':
    main (setup_args ())
