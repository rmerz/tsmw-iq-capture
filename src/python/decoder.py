import numpy as np

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
