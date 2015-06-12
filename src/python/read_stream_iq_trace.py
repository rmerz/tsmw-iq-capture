#!/usr/bin/env python3

import argparse
import numpy as np
from scipy import signal
from matplotlib.pylab import figure, tight_layout, show, savefig
import decoder

def setup_args():
    parser = argparse.ArgumentParser(description='Extract data from binary file obtained with extract_iq.')
    parser.add_argument('filepath', nargs='+', type=str, help='File to open.')
    parser.add_argument('-a','--aggregate', action='store_true', help='Aggregate all blocks together.')
    parser.add_argument('-n','--number_of_blocks', type=int, help='Number of blocks to parse (default is all).')
    parser.add_argument('--header_length', type=int, default=512, help='Length of the text header (default is 512)')
    parser.add_argument('--to-file', type=str, nargs='+', help='Export data to binary file for Gnuradio.')
    parser.add_argument('--plot_timeseries', action='store_true', help='Plot time-series (each channel in a separate window).')
    parser.add_argument('--plot_spectral', action='store_true', help='Plot spectral analysis.')
    parser.add_argument('--plot_iq_power', action='store_true', help='Plot IQ power.')
    parser.add_argument('--plot_angle', action='store_true', help='Plot phase angle.')
    parser.add_argument('-g','--gain-dB', default=0, type=int, help='Increase received signal by gain value (in dB, default to 0).')
    args   = parser.parse_args()
    return args

def spawn_plot ():
    ax = figure ().add_subplot (111)
    return ax

def plot_block_complex (ax,real,imag,linestyle='-'):
    print ('Block length: {:d}'.format (len (real)))

    # Plot real and imaginary part
    ax.plot (real,
             c='m',ls=linestyle,label='RE')
    ax.plot (imag,
             c='Orange',ls=linestyle,label='IM')
    ax.grid (True)
    ax.set_title ('Real and imaginary part plot')
    ax.legend (loc='best')
    tight_layout ()

def plot_block_angle (ax,real,imag,color='m'):
    print ('Block length: {:d}'.format (len (real)))
    # Plot angle
    ax.plot (np.angle (real+1j*imag, deg=True),
             c=color)
    ax.grid (True)
    ax.set_title ('Complex angle plot')
    ax.set_ylim ([-180,180])
    ax.set_yticks (np.arange (-180,181,20))
    tight_layout ()

def plot_block_magnitude (ax,real,imag):
    print ('Block length: {:d}'.format (len (real)))
    mag = np.sqrt (real**2.0+imag**2.0)
    print (len (mag))
    ax.plot (mag,
             c='m')
    ax.grid (True)
    tight_layout ()

def plot_block_iq_power (ax,real,imag,color='m'):
    print ('Block length: {:d}'.format (len (real)))
    iq_power = 10*np.log10( np.power (real,2.0) + np.power (imag,2.0))
    ax.plot (iq_power,
             c=color)
    ax.grid (True)
    ax.set_ylim ([-40,-10])
    ax.set_title ('IQ power')
    tight_layout ()


def plot_block_spectrum (ax,real,imag,sample_rate,color='m'):
    print ('Block length: {:d}'.format (len (real)))
    # Plot spectrum analysis
    # f, Pxx_den = signal.periodogram(real+np.complex(0,1)*imag, nfft=2048)
    f, Pxx_den = signal.welch(real+np.complex(0,1)*imag,
                              fs = sample_rate,
                              #scaling='density',
                              scaling='spectrum',
                              nperseg=4096)
    ax.plot (f, 10*np.log10 (Pxx_den),
             c=color)
    # plt.ylim ([1e-11, 1e-5])
    ax.grid (True)
    tight_layout ()


def open_stream_trace (filepath,header_length):
    f = open (filepath,'rb')

    # This assumes you already know the length of the header
    header = decoder.decode_header (f.read (header_length))
    print ('Header: ',header)

    return f

def decode_stream_trace (f,gain_dB=0,number_of_blocks=None,aggregate=False):
    FE = decoder.decode_fe_freq (f)
    print ('Front-end frequencies: {}'.format (FE))
    number_of_channels = decoder.decode_uint32 (f,2)
    print ('Number of channels: {}'.format (number_of_channels))
    block_size = decoder.decode_uint32 (f)
    print ('Block size: {}'.format (block_size))
    if aggregate:
        real_scaled_full = np.array ([])
        imag_scaled_full = np.array ([])
    k = 0
    while (True):
        print ('Block {:d}'.format (k))
        k += 1
        # This assumes you know what to expect
        start_time_iq = decoder.decode_uint64 (f)
        if len (start_time_iq) == 0:
            break
        print (start_time_iq)
        sample_rate = decoder.decode_float64 (f)
        print (sample_rate)
        scaling = decoder.decode_int16 (f,n=1)  # Scaling
        print (scaling/100)
        scaling = scaling + gain_dB*100
        print (scaling/100)
        real = decoder.decode_float64 (f,n=block_size) # Real
        imag = decoder.decode_float64 (f,n=block_size) # Real
        print ('RE/IM:',real[0],imag[0])

        scaling_lin = np.power (10,(scaling+gain_dB)/2000)
        print (scaling_lin)
        print ('Avg. RX power:',10*np.log10(np.mean (np.power (real*scaling_lin,2.0) + np.power (imag*scaling_lin,2.0))))

        real_scaled = real*scaling_lin
        imag_scaled = imag*scaling_lin
        if aggregate:
            real_scaled_full = np.concatenate ((real_scaled_full,real_scaled))
            imag_scaled_full = np.concatenate ((imag_scaled_full,imag_scaled))
        if number_of_blocks is not None and k == number_of_blocks:
            break
    if aggregate:
        return real_scaled_full,imag_scaled_full,sample_rate
    else:
        return real_scaled,imag_scaled,sample_rate

def main (args):
    print (args.filepath)

    f = open_stream_trace (args.filepath[0],args.header_length)
    real_scaled_0,imag_scaled_0,sample_rate = decode_stream_trace (f,args.gain_dB,args.number_of_blocks,args.aggregate)

    if len (args.filepath) > 1:
        f = open_stream_trace (args.filepath[1],args.header_length)
        real_scaled_1,imag_scaled_1,sample_rate_1 = decode_stream_trace (f,args.gain_dB,args.number_of_blocks,args.aggregate)

    # if args.gain_dB is not None:
    #     gain_lin = np.power (10,args.gain_dB/10)
    #     real_scaled_0 = real_scaled_0*gain_lin
    #     imag_scaled_0 = imag_scaled_0*gain_lin
    #     if len (args.filepath) > 1:
    #         real_scaled_1 = real_scaled_1*gain_lin
    #         imag_scaled_1 = imag_scaled_1*gain_lin

    # Export for gnuradio
    if args.to_file is not None:
        print ('Gnuradio export. Use complex64.')
        print (real_scaled_0.shape,imag_scaled_0.shape)
        (real_scaled_0+1j*imag_scaled_0).astype ('complex64').tofile (args.to_file[0])
        if len (args.filepath) > 1:
            print (real_scaled_1.shape,imag_scaled_1.shape)
            (real_scaled_1+1j*imag_scaled_1).astype ('complex64').tofile (args.to_file[1])

    # Plot the last block
    if args.plot_timeseries:
        ax = spawn_plot ()
        plot_block_complex (ax,real_scaled_0,imag_scaled_0)
        if len (args.filepath) > 1:
            plot_block_complex (ax,real_scaled_1,imag_scaled_1,linestyle='--')

    if args.plot_angle:
        ax = spawn_plot ()
        plot_block_angle (ax,real_scaled_0,imag_scaled_0)
        if len (args.filepath) > 1:
            plot_block_angle (ax,real_scaled_1,imag_scaled_1,color='b')
        savefig ('angle.png',dpi=300)

    if args.plot_iq_power:
        # ax = spawn_plot ()
        # plot_block_magnitude (ax,real_scaled_0,imag_scaled_0)
        ax = spawn_plot ()
        plot_block_iq_power (ax,real_scaled_0,imag_scaled_0)
        if len (args.filepath) > 1:
            plot_block_iq_power (ax,real_scaled_1,imag_scaled_1,color='b')

    if args.plot_spectral:
        ax = spawn_plot ()
        plot_block_spectrum (ax,real_scaled_0,imag_scaled_0,sample_rate)
        if len (args.filepath) > 1:
            plot_block_spectrum (ax,real_scaled_1,imag_scaled_1,sample_rate_1,color='b')

    if args.plot_iq_power:
        if len (args.filepath) > 1:
            ax = spawn_plot ()
            plot_block_iq_power (ax,real_scaled_0+real_scaled_1,imag_scaled_0+imag_scaled_1,color='Violet')

    show ()

if __name__ == '__main__':
    main (setup_args ())
