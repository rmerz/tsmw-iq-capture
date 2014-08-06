"""Provides classes for the arbitrary MIMO waveform processing

sync_pwr_freq_est:  This function synchronizes on the frames and computes
                    the power, frequency offset, and SNR for both channels.

"""
import numpy as np
import logging


class MimoArbWaveForm():
    """Functions

    Main attributes:

    Additional attributes:

    """
    def __init__(self):
        """Constructor

        """
        self._FS = 5e6 # sampling frequency
        self._GI = 16 # samples for guard interval
        self._FFT = 64 # FFT size/samples for OFDM symbol
        self._LLTF = 52 # number of used subcarriers for LLTF
        self._STS = 2 # number of space-time streams
        self._BURSTLEN = 800
        self._GAPLEN = 80
        self._NSYMB = np.int_((self._BURSTLEN-self._GAPLEN)/(self._FFT+self._GI))
        logging.debug('MimoArbWaveForm class created.')

    @property
    def FS(self):
        """Get the sampling frequency"""
        return self._FS

    @FS.setter
    def FS(self, FS):
        """Set the sampling frequency"""
        self._FS = FS

    def sync_pwr_freq_est(self,re1,im1,re2,im2,blk_size,blk_cnt):
        """Synchronize

        Arguments:
        re1, im1: sample vectors with real and imaginary component of RX 1
        re2, im2: sample vectors with real and imaginary component of RX 2

        Return values, all average over the whole input vectors:
        pwr:    two element vector with symbol power per channel
        rsrp:   two element vector with RSRP per channel
        snr:    two element vector with SNR per channel
        foffs:  mean frequency offset

        """
        NAVG = 10 # number of samples to average for finding the gap
        list_foffs = []
        list_rsrp = []
        list_snr = []
        list_pwr = []
        num_of_frames = 0
        #veclen = np.size(re1)
        #while (sidx+self._BURSTLEN+self._GAPLEN)<veclen:
        for n in range(0,blk_cnt):
            bidx = n*blk_size
            # average and compute the power to find the signal gap between bursts
            iqpwr = np.add(np.add(np.power(re1[bidx:bidx+self._BURSTLEN+self._GAPLEN],2),
                                  np.power(im1[bidx:bidx+self._BURSTLEN+self._GAPLEN],2)),
                           np.add(np.power(re2[bidx:bidx+self._BURSTLEN+self._GAPLEN],2),
                                  np.power(im2[bidx:bidx+self._BURSTLEN+self._GAPLEN],2)))
            iqpwrmat = np.reshape(iqpwr,(-1,NAVG))
            idx1 = np.argmin(np.sum(iqpwrmat,axis=1))+1
            sidx = bidx+idx1*NAVG-1;
            while (sidx-bidx+self._BURSTLEN+self._GAPLEN)<blk_size:
                # use the rough estimation for the fine synchronization
                idx, fo_meas = self.mimo_sync(re1[sidx:sidx+self._BURSTLEN+self._GAPLEN],
                                              im1[sidx:sidx+self._BURSTLEN+self._GAPLEN],
                                              re2[sidx:sidx+self._BURSTLEN+self._GAPLEN],
                                              im2[sidx:sidx+self._BURSTLEN+self._GAPLEN])
                idx2 = sidx+idx
                # drop the beginning so the first sample in the complex vector
                # corresponds to the first sample of the frame
                iqcpx1 = np.empty(self._BURSTLEN, dtype=complex)
                iqcpx1.real = re1[idx2:idx2+self._BURSTLEN]
                iqcpx1.imag = im1[idx2:idx2+self._BURSTLEN]
                iqcpx2 = np.empty(self._BURSTLEN, dtype=complex)
                iqcpx2.real = re2[idx2:idx2+self._BURSTLEN]
                iqcpx2.imag = im2[idx2:idx2+self._BURSTLEN]
                # correct the coarse frequency offset
                iqcpx1c = iqcpx1*np.exp(-2j*np.pi*fo_meas/self._FS*np.arange(0,iqcpx1.size))
                iqcpx2c = iqcpx2*np.exp(-2j*np.pi*fo_meas/self._FS*np.arange(0,iqcpx2.size))
                # perform the fine frequency offset estimation
                fo_fine = self.freq_offs_est(iqcpx1c,iqcpx2c)
                # correct the fine frequency offset
                iqcpx1 = iqcpx1c*np.exp(-2j*np.pi*fo_fine/self._FS*np.arange(0,iqcpx1c.size))
                iqcpx2 = iqcpx2c*np.exp(-2j*np.pi*fo_fine/self._FS*np.arange(0,iqcpx2c.size))
                list_foffs.append(fo_meas + fo_fine)
                # estimate the RSRP and SNR
                rsrp1, rsrp2, snr1, snr2 = self.rsrp_snr_est(iqcpx1,iqcpx2)
                list_rsrp.append([rsrp1,rsrp2])
                list_snr.append([snr1,snr2])
                print(rsrp1, rsrp2, snr1, snr2)
                bpwr1 = 10*np.log10(np.sum(np.power(np.abs(
                        iqcpx1[0:self._BURSTLEN-self._GAPLEN]),2))/self._NSYMB)
                bpwr2 = 10*np.log10(np.sum(np.power(np.abs(
                        iqcpx2[0:self._BURSTLEN-self._GAPLEN]),2))/self._NSYMB)
                print('Processed frame {} in block {}, start index {}'.format(num_of_frames+1,n,idx2))
                list_pwr.append([bpwr1,bpwr2])
                sidx = idx2+self._BURSTLEN-np.int_(self._GAPLEN/2)
                num_of_frames += 1
        pwr = np.mean(list_pwr,axis=0)
        rsrp = np.mean(list_rsrp,axis=0)
        snr = np.mean(list_snr,axis=0)
        foffs = np.mean(list_foffs)
        print('Processed {} frames.'.format(num_of_frames))
        return pwr, rsrp, snr, foffs

    def mimo_sync(self,re1,im1,re2,im2):
        """ Synchronize to frame and estimate a rough frequency offset
        With a sampling frequency of 5 MHz, maximum +/-156.25 kHz can be detected.

        Arguments:
        re1, im1: sample vectors with real and imaginary component of RX 1
        re2, im2: sample vectors with real and imaginary component of RX 2
        
        Return values:
        idx:     sample index where the frame starts
        fo_meas: estimated rough frequency offset

        """
        wnd = np.int_(4*(self._GI + self._FFT))
        Nprep = np.int_(self._FFT/2)
        mavg = np.int_(self._FFT/4) # moving average period for power and corr12
        mavg3 = 2*self._FFT # average period for corr3
        if np.size(re1)!=np.size(im1) or np.size(re2)!=np.size(im2) or np.size(re1)!=np.size(re2):
            raise Exception("Vectors re1, im1, re2, im2 do not have the same length!!!")
        if np.size(re1) < (wnd-mavg+mavg3+self._FFT/2):
            raise Exception("Vectors re1, im1, re2, im2 not long enough ({}) to run synchronization (required length={})!!!".format(np.size(re1),wnd-mavg+mavg3+self._FFT/2))
        iqcpx = np.empty(re1.shape, dtype=complex)
        iqcpx.real = (re1+re2)/2
        iqcpx.imag = (im1+im2)/2
        iqdata = np.concatenate((np.zeros(Nprep,),iqcpx))
        power = np.zeros((wnd,1))
        corr12 = np.zeros((wnd,1), dtype=complex)
        corr3 = np.zeros((wnd,1), dtype=complex)
        # perform the autocorrelation on the STF symbols
        for n in range(0, wnd-mavg):
            power[n] = np.real(np.dot(iqdata[n:n+mavg].transpose(),
                        iqdata[n:n+mavg].conjugate())/mavg)
            corr12[n+mavg] = np.sum(iqdata[n+self._FFT/4:n+self._FFT/4+mavg] *
                                np.conj(iqdata[n+self._FFT/2:n+self._FFT/2+mavg]) -
                                iqdata[n:n+mavg] *
                                np.conj(iqdata[n+self._FFT/4:n+self._FFT/4+mavg]))
            corr3[n+mavg] = np.dot(np.transpose(iqdata[n+self._FFT/4:n+self._FFT/4+mavg3]),
                            np.conj(iqdata[n+self._FFT/2:n+self._FFT/2+mavg3]))
        # get first index where power rises above threshold
        idx1 = np.flatnonzero((power>0.75*np.sum(power)/np.size(power)))[0]
        idx2 = np.argmax(np.abs(corr12[idx1:idx1+self._FFT/2]))
        idx = idx1+idx2-Nprep
        c3i = idx1+idx2-Nprep-1+mavg
        # get the phase at the start index and calculate the frequency offset
        fo_meas = -np.angle(np.mean(corr3[c3i:c3i+mavg]))/(np.pi/2*self._FFT)*self._FS
        return idx, fo_meas

    def freq_offs_est(self,iqcpx1,iqcpx2):
        """ Fine estimation of frequency offset
        With a sampling frequency of 5 MHz, maximum +/-16.026 kHz can be detected.

        Arguments:
        iqcpx1: IQ sample stream from RX 1
        iqcpx2: IQ sample stream from RX 2
        NOTE: both vectors need to be synchronized to the frame!

        Return Value:
        foffs: Estimated fine frequency offset

        """
        Nsym = self._FFT+self._GI
        Noffs = 5*Nsym
        NGI = 2
        # autocorrelation over the two MIMO channel sounding symbol pairs
        acf1 = np.sum(iqcpx1[Noffs+2*Nsym+NGI:Noffs+4*Nsym-NGI]*
                np.conjugate(iqcpx1[Noffs+NGI:Noffs+2*Nsym-NGI]))
        acf2 = np.sum(iqcpx2[Noffs+2*Nsym+NGI:Noffs+4*Nsym-NGI]*
                np.conjugate(iqcpx2[Noffs+NGI:Noffs+2*Nsym-NGI]))
        # use the phase to compute the residual frequency offset
        foffs = self._FS/(2*np.pi*(2*Nsym-2*NGI))*(np.angle(acf1)+np.angle(acf2))/2
        return foffs

    def rsrp_snr_est(self,iqcpx1,iqcpx2):
        """ Reference Signal Received Power and SNR estimation

        Arguments:
        iqcpx1: IQ sample stream from RX 1
        iqcpx2: IQ sample stream from RX 2
        
        Return Values:
        rsrp1, rsrp2: Reference Signal Received Power from RX 1 and RX 2
        snr1, snr2:   Estimated SNR from RX 1 and RX 2

        """
        LLTFpos = np.int_(np.concatenate((np.arange((self._FFT-self._LLTF)/2,self._FFT/2),
                                          np.arange(self._FFT/2+1,self._FFT/2+self._LLTF/2+1))))
        # use an offset of -1/2 cyclic prefix
        idx = 4*self._GI+2*self._FFT-self._GI/2
        # extract the two LTF symbols of both channels
        yLLtf1 = iqcpx1[idx:idx+2*self._FFT]
        yLLtf2 = iqcpx2[idx:idx+2*self._FFT]
        # do the FFT on both LLTF pairs
        ysLLtf1a = np.fft.fftshift(np.fft.fft(yLLtf1[0:self._FFT],self._FFT))/np.sqrt(self._STS*self._LLTF)
        ysLLtf1b = np.fft.fftshift(np.fft.fft(yLLtf1[self._FFT:2*self._FFT],self._FFT))/np.sqrt(self._STS*self._LLTF)
        ysLLtf2a = np.fft.fftshift(np.fft.fft(yLLtf2[0:self._FFT],self._FFT))/np.sqrt(self._STS*self._LLTF)
        ysLLtf2b = np.fft.fftshift(np.fft.fft(yLLtf2[self._FFT:2*self._FFT],self._FFT))/np.sqrt(self._STS*self._LLTF)
        # Channel 1
        Ps1 = np.real(np.dot(ysLLtf1a[LLTFpos].conj().transpose(),ysLLtf1a[LLTFpos]))
        Ps2 = np.real(np.dot(ysLLtf1b[LLTFpos].conj().transpose(),ysLLtf1b[LLTFpos]))
        # divide average symbol power by number of symbols and pilot subcarriers
        rsrp1 = 10*np.log10((Ps1+Ps2)/(2*np.size(LLTFpos)))
        Pn = np.sqrt(2)*np.sum(np.power(np.abs(ysLLtf1b[LLTFpos])-np.abs(ysLLtf1a[LLTFpos]),2))
        snr1 = 10*np.log10((Ps1-Pn)/Pn)
        # Channel 2
        Ps1 = np.real(np.dot(ysLLtf2a[LLTFpos].conj().transpose(),ysLLtf2a[LLTFpos]))
        Ps2 = np.real(np.dot(ysLLtf2b[LLTFpos].conj().transpose(),ysLLtf2b[LLTFpos]))
        # divide average symbol power by number of symbols and pilot subcarriers
        rsrp2 = 10*np.log10((Ps1+Ps2)/(2*np.size(LLTFpos)))
        Pn = np.sqrt(2)*np.sum(np.power(np.abs(ysLLtf2b[LLTFpos])-np.abs(ysLLtf2a[LLTFpos]),2))
        snr2 = 10*np.log10((Ps1-Pn)/Pn)
        return rsrp1, rsrp2, snr1, snr2
