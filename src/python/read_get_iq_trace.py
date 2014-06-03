#!/usr/bin/env python

import argparse, sys
import numpy as np
from scipy import signal
from matplotlib import pylab as plt

def setup_args():
    parser = argparse.ArgumentParser(description='Extract data from binary file obtained with get_iq.')
    parser.add_argument('filepath', type=str, help='File to open.')
    parser.add_argument('--append', action='store_true', help='Append all measurement blocks.')
    parser.add_argument('--analysis_mode', type=str, default='spectrum', help='Frequency analysis mode. Default is \'spectrum\' (available is also \'density\'')
    parser.add_argument('-n','--number_of_blocks', type=int, help='Process that much blocks.')
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
    number_of_channels_fe = decode_uint32 (f,2)
    print (number_of_channels_fe)
    number_of_channels = np.sum (number_of_channels_fe)
    block_size = decode_uint32 (f)

    if args.append:
        real_lin_trace_ch1 = np.array([])
        imag_lin_trace_ch1 = np.array([])
        if number_of_channels == 2:
            real_lin_trace_ch2 = np.array([])
            imag_lin_trace_ch2 = np.array([])
        if number_of_channels > 2:
            print ('Append mode not supported for more than two channels.')
            sys.exit (-1)
            

    print ('Block size: {}'.format(block_size))
    block_counter = 1
    while (True):
        start_time_iq = decode_uint64 (f)
        if len (start_time_iq) == 0:
            break
        print ('Block {}'.format(block_counter))
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
        if number_of_channels > 1:
            real_lin_ch2 = real[block_size:]*scaling_lin[1]
            imag_lin_ch2 = imag[block_size:]*scaling_lin[1]
            print (average_iq_power (real_lin_ch2,imag_lin_ch2))

        if args.append:
            real_lin_trace_ch1 = np.append (real_lin_trace_ch1,real_lin_ch1)
            imag_lin_trace_ch1 = np.append (imag_lin_trace_ch1,imag_lin_ch1)
            print ('Channel 1 trace length is {}/{}'.format (len(real_lin_trace_ch1),len(imag_lin_trace_ch1)))
            if number_of_channels == 2:
                real_lin_trace_ch2 = np.append (real_lin_trace_ch2,real_lin_ch2)
                imag_lin_trace_ch2 = np.append (imag_lin_trace_ch2,imag_lin_ch2)
                print ('Channel 2 trace length is {}/{}'.format (len(real_lin_trace_ch1),len(imag_lin_trace_ch1)))
        if args.number_of_blocks is not None and args.number_of_blocks == block_counter:
            break
        block_counter += 1

    f.close ()
    if args.append:
        trace_ch1_iq_power = average_iq_power (real_lin_trace_ch1,imag_lin_trace_ch1)
        print ('Channel 1 trace avg. IQ power: {}'.format (trace_ch1_iq_power))
        if number_of_channels == 2:
            trace_ch2_iq_power = average_iq_power (real_lin_trace_ch2,imag_lin_trace_ch2)
            print ('Channel 2 trace avg. IQ power: {}'.format (trace_ch2_iq_power))


    # Display last block. And see 4.27 in
    # http://www.ni.com/pdf/manuals/370192c.pdf for why spectrum
    # versus density: PSD is PS / (1/sampling_rate * noise_bandwidth)
    if not (args.analysis_mode == 'spectrum' or args.analysis_mode == 'density'):
        print ('Invalid analysis mode.')
        sys.exit (-1)
    if args.append:
        complex_signal_ch1 = real_lin_trace_ch1+np.complex(0,1)*imag_lin_trace_ch1
    else:
        complex_signal_ch1 = real_lin_ch1+np.complex(0,1)*imag_lin_ch1
    f_ch1, Pxx_den_ch1 = signal.welch(complex_signal_ch1,
                                      fs = sample_rate,
                                      scaling=args.analysis_mode,
                                      nperseg=512)
    if number_of_channels == 2:
        if args.append:
            complex_signal_ch2 = real_lin_trace_ch2+np.complex(0,1)*imag_lin_trace_ch2
        else:
            complex_signal_ch2 = real_lin_ch2+np.complex(0,1)*imag_lin_ch2
        f_ch2, Pxx_den_ch2 = signal.welch(complex_signal_ch2,
                                          fs = sample_rate,
                                          scaling=args.analysis_mode,
                                          nperseg=512)

    
    # plt.ion ()
    # plt.figure (1)
    # plt.plot (f_ch1, 10*np.log10 (Pxx_den_ch1))
    # plt.grid (True)
    # plt.title ('Channel 1')
    # plt.tight_layout ()
    # if number_of_channels == 2:
    #     plt.figure (2)
    #     plt.plot (f_ch2, 10*np.log10 (Pxx_den_ch2))
    #     plt.grid (True)
    #     plt.title ('Channel 2')
    #     plt.tight_layout ()
    # input ('Press any key.')

        
    

if __name__ == '__main__':
    main (setup_args ())
