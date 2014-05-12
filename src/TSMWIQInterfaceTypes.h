/**
 * TSMW Project   TSMW IQ Interface DLL
 *
 * @file          TSMWIQInterfaceTypes.h
 * @abstract
 *
 * @copyright     (c) 2007 Rohde & Schwarz GmbH & Co. KG, Munich
 * @author        Markus Herdin, Johannes Dommel
 * @version
 *    06.11.2007  Hd start
 *    21.07.2010  FG Extend TSMW_IQIF_STREAM_CTRL structure for digital I/Q output support of TSMW.
 *    10.09.2010  FB moved and updated digital I/Q output control member to TSMW_IQIF_CH_CTRL
 *
 */

#ifndef  _TSMWIQINTERFACETYPES_H
#define  _TSMWIQINTERFACETYPES_H

#define IQSAMPLINERATE_HZ    (395e6 / 18)

// TSMW IQ interface mode structure
typedef struct TSMW_IQIF_MODE
{
   unsigned char    Frontends;             // Specifies which frontend to enable
                                           //   1: Enable frontend 1
                                           //   2: enable frontend 2,
                                           //   3: enable both frontends
   unsigned long    AMPS_CH1;              // A combination of bits that define which preselector bands (amplifier)
                                           // to activate for rf channel 1. Each bit corresponds to a specific frequency band
                                           //   Bit 0: <0.6GHz
                                           //   Bit 1: 0.6 - 1.2GHz
                                           //   Bit 2: 1.2 - 1.7GHz
                                           //   Bit 3: 1.7 - 2.5GHz
                                           //   Bit 4: >2.5GHz
   unsigned long    AMPS_CH2;              // Same for rf channel 2
   unsigned char    Mode;                  // 0: Standard mode, 1: Calibration mode (shall not be used)
} TSMW_IQIF_MODE_t;

// Filter parameter structure
typedef struct TSMW_IQIF_FILTER_PARAM
{
   unsigned short   FilterID;              // ID of the filter
   unsigned short   NoOfCoeffs;            // Number of filter coefficients
   unsigned char    OvsplFact;             // Filter coefficient oversampling factor
   double           FilterCorrdB;          // Filter gain correction value in the passband in dB. The result will be corrected by this value.
                                           // NOTE: This value is the (negative) remaining filter gain after considering a factor
                                           // of 2^(-ResultShiftNumber)
   unsigned char    ResultShiftNumber;     // No of bits the result is shifted. Compensates for the filter gain.
   unsigned short   GroupDelay;            // Group delay of this filter in taps
   double           Ndown;                 // Downsampling factor this filter was designed for.
} TSMW_IQIF_FILTER_PARAM_t;

// Measurement control structure
typedef struct TSMW_IQIF_MEAS_CTRL
{
   unsigned long        NoOfSamples;       // Number of IQ samples to measure
   unsigned char        FilterType;        // Filter type:
                                           //   0: Use predefined filters
                                           //   1: Use userdefined filters (they have to be sent to the TSMW, beforehand)
   unsigned short       FilterID;          // ID of the filter that shall be used
   unsigned char        DataFormat;        // IQ-data compression format for blockwise compression
                                           //   0: 2 x  8 Bit per complex sample
                                           //   1: 2 x 12 Bit
                                           //   2: 2 x 16 Bit
                                           //   3: 2 x 20 Bit
   unsigned char        AttStrategy;       // Attenuation strategy, currently unused, shall be set to zero
   unsigned char        Splitter;          // RF channel 1 splitter to split signal after preselector to
                                           // both frontends.
                                           //   0: Disable splitter
                                           //   1: Enable splitter
   unsigned short       Priority;          // Relative priority of this meas.req., Valid range: 0 .. 15,
                                           // 0 is lowest priority, 15 highest
} TSMW_IQIF_MEAS_CTRL_t;

// Channel control structure
typedef struct TSMW_IQIF_CH_CTRL
{
   unsigned __int64   Frequency;           // Center frequency in Hz
   unsigned char      UseOtherFrontend;    // Reserved for future use, has to be zero
   unsigned char      NoOfChannels;        // Number of channels that shall be used (1..4). This means the receive
                                           // bandwidth of the selected frontend is split up into the corresponding
                                           // number of channels.
                                           // NOTE: The downsampling factor has to be larger or equal to the
                                           // number of channels. I.e. if 3 channels are used, the downsampling
                                           // factor has to be at least 3.
   char               Attenuation;         // Attenuation to use (0..15dB)
                                           // NOTE: Only the values 0, 5, 10 and 15dB are calibrated
   unsigned char      Preamp;              // Defines whether the preamp shall be enabled or not
                                           //   0: Disable
                                           //   1: Enable
   unsigned char      CalibInput;          // Defines whether the calibration input shall fed to the used frontend
                                           //   0: Disable
                                           //   1: Enable
                                           // NOTE: The calibration amplifier has to be enabled, too (see TSMW_IQIF_MODE)

   double             FreqShift[4];        // Frequency shift from center frequency in Hz for each subchannel
   long               ChannelDelay[4];     // Individual delay in taps for each subchannel (after filtering/resampling)

   long               BlockSize;           // Reserved for future use, has to be zero
   long               BlockSkip;           // Reserved for future use, has to be zero
   unsigned char      DigIqOnOff;          // Configuration for digital I/Q output:
                                           // 0: Digital I/Q output disabled
                                           // 1: Data from Front End mapped on Digital I/Q output
} TSMW_IQIF_CH_CTRL_t;

// IQ result parameter structure
typedef struct TSMW_IQIF_RESULT
{
   unsigned long      MeasRequestID;       // Meas.request ID
   unsigned long      Reserved;
   unsigned __int64   StartTimeIQ;         // IQ-Counter value at which measurement was started.
                                           // NOTE: Each individual channel will have a different start time,
                                           // according to the specified "ChannelDelay" parameter in the
                                           // meas.request structure.

   unsigned __int64   StartTimeHost;       // Corresponding host time

   unsigned __int64   Offset;              // Offset in IQ-samples from measurement start (only for streaming meas.)
   unsigned long      NoOfSamples;         // Number of samples that were copied for each subchannel
   unsigned long      Reserved2;
   double             Fsample;             // Sampling rate that was actually used

   unsigned long      Attenuation[2];      // Attenuation that was used in dB
   unsigned long      Preamp[2];           // 0: Preamp was off, 1: Preamp was on
} TSMW_IQIF_RESULT_t;

// Streaming control structure
typedef struct TSMW_IQIF_STREAM_CTRL
{
   unsigned long        StreamID;          // Stream ID, valid range: 0..15
   unsigned long        StreamBufferSize;  // Buffer size for streaming in MBytes, 200MB is recommended
   unsigned long        MaxStreamSize;     // Maximum streaming size in MBytes. Stream output files are
} TSMW_IQIF_STREAM_CTRL_t;

typedef struct TSMW_IQIF_STREAM_STATUS
{
   unsigned char        Status;            // Streaming status
                                           // 0:     Not activated
                                           // 1:     Running
                                           // 2:     Finished
                                           // 3:     Stopped because of error
                                           // 4:     Maximum streaming (file) size reached

   unsigned long        BlockSize;         // No of samples per streaming block
   unsigned long        NoOfBlocks;        // Number of received blocks
   unsigned long        NoOfSkippedBlocks; // Number of skipped blocks
} TSMW_IQIF_STREAM_STATUS_t;

// Streaming information structure
typedef struct TSMW_IQIF_STREAM_INFO
{
   // Date and time when streaming was started
   unsigned long        Year;
   unsigned long        Month;
   unsigned long        Date;
   unsigned long        Hour;
   unsigned long        Minute;
   unsigned long        Second;

   char                 Description[256];  // Description

   unsigned __int64     NoOfFiles;         // Number of files for this stream
   unsigned long        StreamSize;        // Total stream size in MB

   unsigned long        BlockSize;         // No of samples per streaming block
   unsigned long        NoOfBlocks;        // Number of received blocks
   unsigned long        NoOfSkippedBlocks; // Number of skipped blocks
} TSMW_IQIF_STREAM_INFO_t;

typedef struct TSMW_IQIF_RESOURCE_PARAM
{
   unsigned char    Frontends;             // Specifies which frontend to use
                                           //   1: Frontend 1
                                           //   2: Frontend 2,
                                           //   3: Both frontends
   unsigned char    Capacity;              // Requested receiver capacity in %. The actual percentage
                                           // a resource will get is given by
                                           // (Capacity / <Sum capacity of all resources>)
                                           // The sum capacity can be larger than 100%
   unsigned long    ObservationTime;       // Observation time window in ms. Only measurements that were
                                           // taken in the last <ObservationTime> ms will count for
                                           // the current resource usage.
} TSMW_IQIF_RESOURCE_PARAM_t;

typedef struct TSMW_IQIF_SCHEDULE_CTRL
{
   unsigned long    ResourceID;            // Resource ID the scheduler shall use
   unsigned long    SchedulerTimeConst;    // Scheduler time constant for non-periodic measurements in ms
                                           // The priority of a non-periodic measurement is given by
                                           // <Priority in meas.req.> + <Waiting time in ms> / <SchedulerTimeConst>.
                                           // This means that the priority of a meas.req. is increased with
                                           // longer waiting time.
} TSMW_IQIF_SCHEDULE_CTRL_t;

typedef struct TSMW_IQIF_PERIOD_CTRL
{
   unsigned long    Cmd;                   // Command:
                                           //   0: Start period. meas.req. (when used in TSMWIQMeasure_c)
                                           //      Otherwise, add credits to period. meas.req. without changing parameters
                                           //   1: Clear all credits of period. meas.req (de-activate it)
                                           //   2: Cancel period. meas.req.
                                           //   3: Change parameters
                                           //   4: Adjust time slip
                                           //   5: Change attenuator and preamp setting
   unsigned long    MeasRequestID;         // Meas.request ID of period. meas.req., only used when changing
                                           // parameters of a period. meas.req.
   unsigned __int64 IQPeriod;              // Time period in IQ samples. Minimum time period is 1ms -> 22000 I/Q samples
   unsigned __int64 IQMinDistance;         // Minimum distance between consecutive measurements in I/Q samples,
                                           // only used if no start time is given.
   unsigned __int64 GridTimeus;            // Time grid in microseconds on which measurements shall be performed. Is only used
                                           // when a start time is given. In that case, measurements are only performed on a time
                                           // grid given by StartTime + n * GridTime.
                                           // NOTE: GridTimeus is converted, internally, from microseconds to I/Q-samples.
                                           // The StartTime is always given in I/Q-samples
   short            IQTimeSlip;            // Time slip for periodic measurement requests with a start time, only
                                           // used when Cmd == 4. The grid at which a measurement will be started is
                                           // adjusted by the IQTimeSlip. NOTE: The time slip is given in I/Q samples
                                           // before resampling.
   unsigned short   NoOfMeasurements;      // Number of measurements to deliver, initially, until new
                                           // "measurement credits" have to be added via Cmd = 0.
   unsigned char    TransmitWindow;        // Transmit window (between 2 and 16). Gives the number of measurements
                                           // that are performed until the first has to be transmitted to the host.
                                           // This prohibits the TSMW of making more measurements than can be
                                           // transmitted to the host (because of limited ethernet capacity).
   unsigned char    Att[2];                // New attenuator setting for each channel (only used when Cmd == 5)
   unsigned char    Preamp[2];             // New preamp setting for each channel (only used when Cmd == 5)
} TSMW_IQIF_PERIOD_CTRL_t;

typedef struct TSMW_IQIF_TRIG_CTRL {
   unsigned long    Cmd;                   // 0: Start triggering (when used in TSMWIQMeasureTrig command)
                                           // 1: Stop triggered measurement
                                           // 2: Change attenuator and preamp setting
   unsigned char    Mode;                  // Trigger mode, has to be zero
   unsigned char    Falling;               // Trigger edge, 0: rising, 1: falling
   unsigned char    TriggerLine;           // 1: Use trigger input 1
                                           // 2: Use trigger input 2
                                           // 3: Trigger on both inputs
   unsigned long    MeasRequestID;         // Meas.request ID of period. meas.req., only used when changing
                                           // parameters of a triggered measurement
   unsigned char    Att[2];                // New attenuator setting for each channel (only used when Cmd == 2)
   unsigned char    Preamp[2];             // New preamp setting for each channel (only used when Cmd == 2)
} TSMW_IQIF_TRIG_CTRL_t;

#endif // _TSMWIQINTERFACETYPES_H

