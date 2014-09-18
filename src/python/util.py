import numpy as np

def moving_average (x, n, type='simple'):
    """
    compute an n period moving average.

    type is 'simple' | 'exponential'

    """
    x = np.asarray (x)
    if type=='simple':
        weights = np.ones (n)
    else:
        weights = np.exp (np.linspace (-1., 0., n))

    weights /= weights.sum ()

    a =  np.convolve (x, weights, mode='full')[:len (x)]
    a[:n] = a[n]
    return a
