/**
 * TSMW Project   TSMW IQ Interface DLL
 *
 * @file          TSMWIQInterfaceFunc.h
 * @abstract
 *
 * @copyright     (c) 2007 Rohde & Schwarz GmbH & Co. KG, Munich
 * @author        Markus Herdin, Johannes Dommel
 * @version
 *    06.11.2007  Hd start
 *    06.12.2010  FB added function TSMWGetStreamError_c.
 *    17.02.2011  FB added new functions to control the GPS server:
 *                     - TSMWGPSStartTcpServer_c
 *                     - TSMWGPSStopServer_c
 *                     - TSMWGPSGetServerStatus_c
 *    08.07.2011  FB Added new functions to receive the Digital I/Q Interface
 *                  status message:
 *                     - TSMWINFOIFEnable_c
 *                     - TSMWINFOIFGetStatus_c
 *
 */

#include "TSMWIQInterfaceTypes.h"

#ifndef  _TSMWIQINTERFACEFUNC_H
#define  _TSMWIQINTERFACEFUNC_H

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------
/**
 * Get TSMW IQ Interface version.
 *
 * @return int    32-Bit integer representing the 4-byte version code Major.Minor.Patch.QFE
 */
int __stdcall TSMWGetVersion_c ( void );
//--------------------------------------------------------------------------------------------
/**
 * Get TSMW IQ Interface version as text
 *
 * @return char   Version as text
 */
char* __stdcall TSMWGetVersionText_c ( void );
//--------------------------------------------------------------------------------------------
/**
 * Initialize the TSMW IQ Data Interface DLL. Has to be called before any other function is called.
 *
 * @return int    Errorcode, 0 if successful
 */
int __stdcall TSMWInitInterface_c ( void );
//--------------------------------------------------------------------------------------------
/** Release the TSMW IQ Data Interface DLL. Has to be called before the dll is unloaded.
 *
 * @return int    Errorcode, 0 if successful
 */
int __stdcall TSMWReleaseInterface_c ( void );
//--------------------------------------------------------------------------------------------
/**
 * Get error message and error code. Each function returns an error code, which is equal to zero
 * if no error occured. If an error occured, the corresponding error text is saved and can be
 * aquired by this function.
 *
 * Note: The error code is only valid until another function has been called.
 *
 * @param pErrorCode    OUT: Pointer to int which will receive the error code. Error code will be
 *                      zero if successful
 *
 * @return char*        Pointer to zero-terminated string with error message.
 */
char* __stdcall TSMWGetLastError_c ( int *pErrorCode );
//--------------------------------------------------------------------------------------------
/**
 * Connect to TSMW with given IP address
 *
 * @param IPAddress     IN:  IP address as string
 * @param pTSMWMode     IN:  Pointer to TSMW mode structure
 * @param pTSMWID       OUT: Pointer to unsigned short which receives the TSMW ID
 *
 * @return int          Errorcode, 0 if successful
 */
int __stdcall TSMWConnect_c (char* IPAddress, TSMW_IQIF_MODE_t *pTSMWMode, unsigned short *pTSMWID);
//--------------------------------------------------------------------------------------------
/**
 * Addes/modifies filter specifications for an existing measurement session. If a filter specification
 * with the given ID has already been sent to the TSMW, a new call of TSMWIQSetup_c with the same
 * filter ID will overwrite the previous filter specification. This however is not the case if
 * the previous filter specification is used by a different application (with a different
 * IP connection to the TSMW).
 *
 * @param TSMWID           IN: TSMW ID
 * @param pFilterSpec      IN: Pointer to TSMW_IQIF_FILTER_PARAM_t structure containing filter specification.
 * @param plCoeff          IN: Pointer to filter coefficients
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQSetup_c (unsigned short TSMWID,
                             TSMW_IQIF_FILTER_PARAM_t *pFilterParam,
                             long *plCoeff);
//--------------------------------------------------------------------------------------------
/**
 * Request receiver resources.
 *
 * @param TSMWID           IN: TSMW ID
 * @param IsRequest        IN: 1: Request receiver resource
 *                             0: Release receiver resource
 * @param pResourceID      OUT (if resource is requested): receives resource id
 *                         IN  (if resource is released) : specifies resource id to release
 * @param pParam           IN: Resource parameter structure
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQResourceRequest_c (unsigned short TSMWID, unsigned long IsRequest,
                                       unsigned long *pResourceID, TSMW_IQIF_RESOURCE_PARAM_t *pParam);
//--------------------------------------------------------------------------------------------
/**
 * Start a new measurement. Measurements are scheduled on the TSMW. The measurement specification
 * includes a priority and might also include (several) start times at which the measurement can
 * be started. The TSMW schedules automatically the measurement request with the highest priority
 * and the earliest start time. Start times given by the pStartTimes parameter are exclusive, i.e.
 * only a single measurement is scheduled but any of the given start times can be used. If no start
 * times are given, the measurement is scheduled as early as possible.
 *
 * @param TSMWID           IN: ID of TSMW to use for measurement
 * @param pMeasRequestID   OUT: Pointer to MeasRequestID variable, will receive the measurement
 *                         request ID.
 * @param pStartTimes      IN: Pointer to array with start times (optional), if not used: NULL
 * @param NoOfStartTimes   IN: Number of start times given, if no start time given: 0
 * @param pMEAS_CTRL       IN: Measurement control structure
 * @param pCHANNEL_CTRL1   IN: Channel control structure for RF channel 1, NULL if RF channel 1 is
 *                         not used by this meas. req.
 * @param pCHANNEL_CTRL2   IN: Channel control structure for RF channel 2, NULL if RF channel 2 is
 *                         not used by this meas.req.
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQMeasure_c (unsigned short TSMWID, unsigned long *pMeasRequestID,
                               unsigned __int64 *pStartTimes, long NoOfStartTimes,
                               TSMW_IQIF_MEAS_CTRL_t *pMEAS_CTRL,
                               TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL1,
                               TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL2 );
//--------------------------------------------------------------------------------------------
/**
 * Start an extended measurement. Similar to TSMWIQMeasure_c, but allows to specify scheduling
 * parameters and/or to start periodic measurement requests.
 *
 * @param TSMWID
 * @param pMeasRequestID
 * @param pStartTimes
 * @param NoOfStartTimes
 * @param pMEAS_CTRL
 * @param pCHANNEL_CTRL1
 * @param pCHANNEL_CTRL2
 * @param pSchedParam      IN: Scheduling parameter structure. Has to be specified if the
 *                         measurement request shall use a specific receiver resource class
 *                         that was previously allocated via TSMWIQResourceRequest_c.
 *                         If == NULL, no receiver resource class will be used.
 *                         NOTE: Measurements that do not use a specific receiver resource class
 *                         will not be considered for receiver capacity limitation!
 * @param pPeriodParam     IN: Control structure for starting periodic meas.req.s. If == NULL,
 *                         this is a standard measurement, if given, a periodic measurement request
 *                         is given.
 *
 * @return int
 */
int __stdcall TSMWIQMeasureExt_c (unsigned short TSMWID, unsigned long *pMeasRequestID,
                                  unsigned __int64 *pStartTimes, long NoOfStartTimes,
                                  TSMW_IQIF_MEAS_CTRL_t *pMEAS_CTRL,
                                  TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL1,
                                  TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL2,
                                  TSMW_IQIF_SCHEDULE_CTRL_t *pSchedParam,
                                  TSMW_IQIF_PERIOD_CTRL_t *pPeriodParam );
//--------------------------------------------------------------------------------------------
// Start a triggered I/Q measurement
int __stdcall TSMWIQMeasureTrig_c (unsigned short TSMWID, unsigned long *pMeasRequestID,
                                   unsigned long long *pStartTimes, long NoOfStartTimes,
                                   TSMW_IQIF_MEAS_CTRL *pMEAS_CTRL,
                                   TSMW_IQIF_CH_CTRL *pCHANNEL_CTRL1,
                                   TSMW_IQIF_CH_CTRL *pCHANNEL_CTRL2,
                                   TSMW_IQIF_TRIG_CTRL_t *pTriggerParam );
//--------------------------------------------------------------------------------------------
int __stdcall TSMWIQPeriodCtrl_c (unsigned short TSMWID, TSMW_IQIF_PERIOD_CTRL_t *pParam);
//--------------------------------------------------------------------------------------------
int __stdcall TSMWIQTriggerCtrl_c (unsigned short TSMWID, TSMW_IQIF_TRIG_CTRL_t *pParam);
//--------------------------------------------------------------------------------------------
/**
 * Start a streaming measurement. Both "online" and "offline" streaming is possible.
 * "Online" streaming means that stream data is processed online, hence stream data is only buffered
 * in memory (up to the given stream buffer size) until it is aquired via one of the TSMWIQGetStreamXXX
 * functions. As soon as the corresponding data has been aquired, the corresponding memory is automatically free'd.
 *
 * "Offline" streaming means that data is automatically written to a stream data file. After the streaming has
 * finished, the stream data file can be opened and read.
 *
 * Streaming can in general be started with the same parameters as normal (block) measurements. However, bandwidth
 * (data rate) limitations have to be considered for successful streaming without lost data blocks. See TSMW IQ
 * Interface Manual (Option K1).
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param pMEAS_CTRL       IN: Measurement control structure
 * @param pCHANNEL_CTRL1   IN: Channel control structure for RF channel 1, NULL if RF channel 1 is
 *                         not used by this meas. req.
 * @param pCHANNEL_CTRL2   IN: Channel control structure for RF channel 2, NULL if RF channel 2 is
 *                         not used by this meas.req.
 * @param pStreamCtrl      IN: Stream control structure
 * @param pFileName        IN: Filename to use for streaming. If omitted (== NULL) online streaming is started.
 *                         During online streaming, stream data blocks up to the specified stream buffer size
 *                         are stored in memory. Data has to be aquired via the TSMWIQGetStreamXXX functions. Data
 *                         handling is automatic, this means that as soon as a stream data block has been read, it is
 *                         automatically free'd.
 *                         If a file name is given, "offline" streaming is started. This means that the stream data
 *                         blocks are automatically stored in a file with the file name <pFileName>_XXX.dat. Here, _XXX
 *                         starts at 000 and counts up. Files are automatically split if they would exceed 2GB file size.
 * @param pDescription     IN: Description that is stored in streaming file
 * @param Flags            IN: Flags:
 *                             0: Do not overwrite existing stream data files
 *                             1: Overwrite existing stream data files
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQStream_c (unsigned short TSMWID,
                              TSMW_IQIF_MEAS_CTRL_t *pMEAS_CTRL,
                              TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL1,
                              TSMW_IQIF_CH_CTRL_t *pCHANNEL_CTRL2,
                              TSMW_IQIF_STREAM_CTRL_t *pStreamCtrl,
                              char *pFileName, char *pDescription,
                              unsigned int Flags);
//--------------------------------------------------------------------------------------------
/**
 * Stop a streaming measurement
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param StreamID         IN: ID of the corresponding stream
 * @param pStreamStatus    IN: Resulting status (see structure description)
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQStopStreaming_c (unsigned short TSMWID, unsigned char StreamID,
                                     TSMW_IQIF_STREAM_STATUS_t *pStreamStatus);
//--------------------------------------------------------------------------------------------
/**
 * Get error message associated with the last error occuring on the streaming measurement.
 *
 * @param StreamID         IN: ID of the corresponding stream
 *
 * @return char*        Pointer to zero-terminated string with error message.
 */
char* __stdcall TSMWGetStreamError_c (unsigned char StreamID);
//--------------------------------------------------------------------------------------------
/**
 * Get current status of streaming measurement.
 *
 * @param StreamID         IN: ID of the corresponding stream
 * @param pStreamStatus    IN: Resulting status (see structure description)
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQStreamStatus_c (unsigned char StreamID,
                                    TSMW_IQIF_STREAM_STATUS_t *pStreamStatus);
//--------------------------------------------------------------------------------------------
/**
 * Open a stream data file
 *
 * @param pFileName        IN: Filename of the stream data file without _xxx.dat extension
 * @param StreamID         IN: Stream ID to use for working with this stream data file
 * @param pStreamInfo      OUT: Pointer to stream info structure, will receive information about
 *                         recorded stream
 * @param pMeasCtrl        OUT: Pointer to meas.control structure, will receive meas.ctrl structure
 *                         that was used when the stream was started
 * @param pChannelCtrl1    OUT: Pointer to channel ctrl. structure, will receive channel control
 *                         structure that was used when the stream was started. If this RF frontend
 *                         was not used during streaming, the field NoOfChannels will be set to zero
 * @param pChannelCtrl2    OUT: Pointer to channel ctrl. structure for RF channel 2, see pChannelCtrl1
 * @param pFilterSpec      OUT: Pointer to filter specification structure, will receive filter spec.
 *                         that was used during streaming
 * @param pCoeff           OUT: Pointer to coefficient vector, will receive filter coefficients. The maximum
 *                         number of filter coefficients is 3096.
 * @param NoOfCoeffs       IN: Number of filter coefficients that can be stored in pCoeff array
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQOpenStreamFile_c (char *pFileName, unsigned char StreamID,
                                      TSMW_IQIF_STREAM_INFO_t *pStreamInfo,
                                      TSMW_IQIF_MEAS_CTRL_t *pMeasCtrl,
                                      TSMW_IQIF_CH_CTRL_t *pChannelCtrl1,
                                      TSMW_IQIF_CH_CTRL_t *pChannelCtrl2,
                                      TSMW_IQIF_FILTER_PARAM_t *pFilterSpec,
                                      long *pCoeff, unsigned long NoOfCoeffs);
//--------------------------------------------------------------------------------------------
/**
 * Close a stream data file.
 *
 * @param StreamID         IN: ID of the corresponding stream
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQCloseStreamFile_c (unsigned char StreamID);
//--------------------------------------------------------------------------------------------
/**
 * Check how many measurement results are available.
 *
 * @param pNoOfIQResults   OUT: Pointer to long which receives the number of IQ measurement results
 *                         available
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQDataAvailable_c (long *pNoOfIQResults);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result parameters.
 * NOTE: This function will not delete the corresponding measurement result. This means that it can be called
 * before one of the TSMWIQGetDataXXX functions is called. The "Get" functions, however, will delete the measurement
 * result.
 *
 * @param MeasRequestID    IN: Measurement request ID for which parameters are requested.
 *                         If the measurement result parameters of the next available measurement will be
 *                         delivered.
 * @param TimeOut          IN: Max. timeout in ms to wait for measurement result
 * @param pIQResult        OUT: Pointer to measurement result parameter structure.
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQGetResultParam_c (unsigned int MeasRequestID, unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in Int16 format.
 * NOTE: This function will delete the corresponding measurement result.
 *
 * @param TSMWID           IN: ID of TSMW
 * @param MeasRequestID    IN: Measurement request ID for which parameters are requested.
 *                         If the measurement result parameters of the next available measurement will be
 *                         delivered.
 * @param TimeOut          IN: Max. time in ms to wait for result
 * @param pIQResult        OUT: Pointer to TSMW_IQIF_RESULT structure, will be filled with result parameters,
 *                         see also TSMWIQGetResultParam_c
 * @param pReal            OUT: Pointer to (NoOfSamples x NoOfChannels) short array to receive real part of data.
 *                         The first NoOfSamples elements in pReal[i] will receive the data of channel 1, the next
 *                         NoOfSamples the data of channel 2 etc.
 * @param pImag            OUT: ... imag part
 * @param pScaling         OUT: Pointer to (1 x NoOfChannels) short array to receive
 *                         scaling factor of each channel. This is the reference level in 1/100 dB. I.e. to get the
 *                         complex sample i in dBm one has to calculate (pReal[i] + pImag[i]) * 10^(Scaling/2000).
 * @param pOvfl            OUT: Pointer to (1 x NoOfChannels) short array to receive overflow indicator of each channel
 *                         Is approximately the number of overflows that occured for in the result data (for each channel)
 * @param pCalibrated      OUT: Pointer to (1 x NoOfChannels) short array to receive calibration indicator of each channel
 *                         0: Channel is not calibrated for this setting
 *                         1: Channel is calibrated for this setting
 * @param NoOfSamples      IN: Number of samples reserved in sample buffer, can be smaller than the number of actually measured
 *                         samples.
 *                         NOTE: Since the measurement result will be deleted when this function is called, the samples
 *                         that were not copied, are lost.
 * @param NoOfChannels     IN: Number of channels reserved in sample buffer, has to be equal to the number of channels that
 *                         were requested in the measurement request
 * @param Reserved1        Has to be 0
 * @param Reserved2        Has to be 0
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQGetDataInt16_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                    unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                    short* pReal, short* pImag, short* pScaling, unsigned long* pOvfl,
                                    unsigned int *pCalibrated,
                                    unsigned int NoOfSamples, unsigned int NoOfChannels,
                                    int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in Int32 format.
 * NOTE: This function will delete the corresponding measurement result.
 *
 * See TSMWIQGetDataInt16_c
 */
int __stdcall TSMWIQGetDataInt32_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                    unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                    long* pReal, long* pImag, short* pScaling, unsigned long* pOvfl,
                                    unsigned int *pCalibrated,
                                    unsigned int NoOfSamples, unsigned int NoOfChannels,
                                    int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in single precision floating point format.
 * NOTE: This function will delete the corresponding measurement result.
 *
 * See TSMWIQGetDataInt16_c
 */
int __stdcall TSMWIQGetDataSingle_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                     unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                     float* pReal, float* pImag, short* pScaling, unsigned long* pOvfl,
                                     unsigned int *pCalibrated,
                                     unsigned int NoOfSamples, unsigned int NoOfChannels,
                                     int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in double precision floating point format.
 * NOTE: This function will delete the corresponding measurement result.
 *
 * See TSMWIQGetDataInt16_c
 */
int __stdcall TSMWIQGetDataDouble_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                     unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                     double* pReal, double* pImag, short* pScaling, unsigned long* pOvfl,
                                     unsigned int *pCalibrated,
                                     unsigned int NoOfSamples, unsigned int NoOfChannels,
                                     int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in interleaved single precision floating point format.
 * This means that the I and Q components are copied to pCplxData in interleaved fashion.
 * Interleaving is such that even index elements pCplxData[i] (i starting with zero) will
 * receive the real part and odd index elements the imaginary part. The ordering of channels
 * is the same is for TSMWIQGetDataInt16_c.
 *
 * This means that (2 x NoOfSamples x NoOfChannels) elements have to be received in pCplxData.
 *
 * NOTE: This function will delete the corresponding measurement result.
 *
 * See TSMWIQGetDataInt16_c
 */
int __stdcall TSMWIQGetDataSingleIlv_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                        unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                        float* pCplxData, short* pScaling, unsigned long* pOvfl,
                                        unsigned int *pCalibrated,
                                        unsigned int NoOfSamples, unsigned int NoOfChannels,
                                        int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
/**
 * Get measurement result data in interleaved double precision floating point format.
 * NOTE: This function will delete the corresponding measurement result.
 *
 * See TSMWIQGetDataSingleIlv_c and TSMWIQGetDataInt16_c
 */
int __stdcall TSMWIQGetDataDoubleIlv_c (unsigned short TSMWID, unsigned int MeasRequestID,
                                        unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                        double* pCplxData, short* pScaling, unsigned long* pOvfl,
                                        unsigned int *pCalibrated,
                                        unsigned int NoOfSamples, unsigned int NoOfChannels,
                                        int Reserved1, int Reserved2);
//--------------------------------------------------------------------------------------------
/**
 * Get stream data in single precision floating point format. This functions works for both
 * "online" streaming and "offline" streaming. "Online" streaming means processing stream data
 * on the fly, hence no automatic writing to a stream data file. "Offline" streaming means that
 * data is recorded on the disc in a stream data file. After the streaming has been finished, the
 * stream data file can be opened and data can be read from the file using this function.
 *
 * @param StreamID         IN: ID of the corresponding stream
 * @param TimeOut          IN: Max. time in ms to wait for result
 * @param pIQResult        OUT: Pointer to TSMW_IQIF_RESULT structure, will be filled with result parameters,
 *                         see also TSMWIQGetResultParam_c
 * @param pReal            OUT: Pointer to (NoOfSamples x NoOfChannels) float array to receive real part of data.
 *                         The first NoOfSamples elements in pReal[i] will receive the data of channel 1, the next
 *                         NoOfSamples the data of channel 2 etc.
 * @param pImag            OUT: ... imag part
 * @param pScaling         OUT: Pointer to (1 x NoOfChannels) short array to receive
 *                         scaling factor of each channel. This is the reference level in 1/100 dB. I.e. to get the
 *                         complex sample i in dBm one has to calculate (pReal[i] + pImag[i]) * 10^(Scaling/2000).
 * @param pOvfl            OUT: Pointer to (1 x NoOfChannels) short array to receive overflow indicator of each channel
 *                         Is approximately the number of overflows that occured for in the result data (for each channel)
 * @param pCalibrated      OUT: Pointer to (1 x NoOfChannels) short array to receive calibration indicator of each channel
 *                         0: Channel is not calibrated for this setting
 *                         1: Channel is calibrated for this setting
 * @param Offset           Only used during "offline" streaming, i.e. reading from a recorded stream data file.
 *                         This is the sample index offset at which to start to get data. 0 means start at the very first
 *                         sample that was recorded.
 * @param NoOfSamples      IN: Number of samples to copy
 * @param NoOfChannels     IN: Number of channels reserved in sample buffer, has to be equal to the number of channels that
 *                         were requested in the measurement request
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWIQGetStreamSingle_c (unsigned char StreamID, unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                       float* pReal, float* pImag, short* pScaling, unsigned long* pOvfl,
                                       unsigned int *pCalibrated, unsigned __int64 Offset,
                                       unsigned int NoOfSamples, unsigned int NoOfChannels);
//--------------------------------------------------------------------------------------------
/**
 * Get streaming data block in double format.
 *
 * See TSMWIQGetStreamSingle_c and TSMWIQGetDataInt16_c.
 */
int __stdcall TSMWIQGetStreamDouble_c (unsigned char StreamID, unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                       double* pReal, double* pImag, short* pScaling, unsigned long* pOvfl,
                                       unsigned int *pCalibrated, unsigned __int64 Offset,
                                       unsigned int NoOfSamples, unsigned int NoOfChannels);
//--------------------------------------------------------------------------------------------
/**
 * Get streaming data block in interleaved single (float) format.
 *
 * See TSMWIQGetStreamSingle_c, TSMWIQGetDataSingleIlv_c and TSMWIQGetDataInt16_c.
 */
int __stdcall TSMWIQGetStreamSingleIlv_c (unsigned char StreamID, unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                          float* pCplxData, short* pScaling, unsigned long* pOvfl,
                                          unsigned int *pCalibrated, unsigned __int64 Offset,
                                          unsigned int NoOfSamples, unsigned int NoOfChannels);
//--------------------------------------------------------------------------------------------
//
/**
 * Get streaming data block in interleaved double format.
 *
 * See TSMWIQGetStreamSingle_c, TSMWIQGetDataSingleIlv_c and TSMWIQGetDataInt16_c.
 */
int __stdcall TSMWIQGetStreamDoubleIlv_c (unsigned char StreamID, unsigned int TimeOut, TSMW_IQIF_RESULT_t *pIQResult,
                                          double* pCplxData, short* pScaling, unsigned long* pOvfl,
                                          unsigned int *pCalibrated, unsigned __int64 Offset,
                                          unsigned int NoOfSamples, unsigned int NoOfChannels);
//--------------------------------------------------------------------------------------------
/**
 * Shutdown TSMW
 */
int __stdcall TSMWShutdown_c (unsigned short TSMWID);
//--------------------------------------------------------------------------------------------
/**
 * Enable tracking generator for selected frontends.
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param Frontends        IN: Selected frontends (tracking generator, i.e. test out is only possible
 *                             for frontend 1):
 *                               0: Test out disabled
 *                               1: Test out for frontend 1 enabled
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWTGEnable_c (unsigned short TSMWID, unsigned int Frontends);
//--------------------------------------------------------------------------------------------
/**
 * Enable external reference.
 * NOTE: One has to ensure that the external reference is already connected when calling this function
 * to enable usage of the external reference. If the external reference has been enabled but there is
 * no external reference clock connected to the TSMW, measurements on rf channel 2 will not work.
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param Enable           IN: 0: Enable external reference
 *                             1: Disable external reference
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWExtRefEnable_c (unsigned short TSMWID, int Enable);
//--------------------------------------------------------------------------------------------
/**
 * Set reference oscillator DAC for frequency correction. Only possible if GPS synchronization
 * has been disabled.
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param Value            IN: DAC value to set (0..1023, sensible range is 200 - 800)
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWSetDAC_c (unsigned short TSMWID, short Value);
//--------------------------------------------------------------------------------------------
/**
 * Enable/disable GPS synchronization. Default: enabled.
 *
 * @param TSMWID           IN: ID of TSMW to use
 * @param Enable           IN: TRUE -> Enable, FALSE -> Disable
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSSync_c (unsigned short TSMWID, int Enable);
//--------------------------------------------------------------------------------------------
/**
 * Enable/disable GPS data subscription. NMEA and UBX GPS data lines will be received from the TSMW and
 * are stored in a ring buffer of the given size. If the buffer size is exceeded, the oldest
 * received NMEA string or UBX message is overwritten. The NMEA and UBX buffers are independent.
 *
 * @param TSMWID           IN: ID of TSMW to use
 * @param NoOfBufferedLines  IN: Number of buffered NMEA lines
 * @param Enable           IN: TRUE -> Enable, FALSE -> Disable
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSEnable_c (unsigned short TSMWID, int NoOfBufferedLines, int Enable);
//--------------------------------------------------------------------------------------------
/**
 * Send NMEA command string to GPS
 *
 * @param TSMWID           IN: ID of TSMW to use
 * @param pNMEAString      IN: NMEA string to send to TSMW
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSSendNMEACmd_c(int TSMWID, char* pNMEAString);
//--------------------------------------------------------------------------------------------
/**
 * Send UBX message to GPS
 *
 * @param TSMWID           IN: ID of TSMW to use
 * @param pUBXMsg          IN: UBX message to send to TSMW
 * @param UBXMsgLength     IN: UBX message length
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSSendUBXCmd_c(int TSMWID, void* pUBXMsg, unsigned int UBXMsgLength);
//--------------------------------------------------------------------------------------------
/**
 * Clear GPS NMEA and UBX buffer.
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSClearBuffer_c ( void );
//--------------------------------------------------------------------------------------------
/**
 * Get next available GPS NMEA data line. Returns an empty line if no NMEA strings is available.
 * This function can be called from another thread than all the other functions. However, it is
 * not possible to call this function from more than one thread!
 *
 * @return char*           Pointer to NMEA text string, valid until next call of function.
 */
char* __stdcall TSMWGPSGetNMEALine_c ();
//--------------------------------------------------------------------------------------------
/**
 * Get next available GPS UBX message. Returns NULL if no UBX message available. This function
 * can be called from another thread than all the other functions. However, it is not possible
 * to call this function from more than one thread!
 *
 * @param pLength          OUT: Pointer to unsigned int to receive length of UBX message
 *
 * @return void*           Pointer to UBX message, valid until next call of function
 */
void* __stdcall TSMWGPSGetUBXMsg_c (unsigned int *pLength);
//--------------------------------------------------------------------------------------------
/**
 * Get estimate of current IQ-time at TSMW (Current IQ-counter value). The IQ-counter value
 * runs at TSMW sample rate (21.9444MHz). The IQ-time has 48 valid bits. The IQ-time is the time
 * that is used to start measurements.
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param pIQTime          OUT: Pointer to __int64 to receive current IQ time.
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGetIQTime_c (unsigned short TSMWID, unsigned __int64 *pIQTime);
//--------------------------------------------------------------------------------------------
/**
 * Enable/disable Digital I/Q Info Interface subscription.
 *
 * @param TSMWID           IN: ID of TSMW to use
 * @param Enable           IN: TRUE -> Enable, FALSE -> Disable
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWINFOIFEnable_c (unsigned short TSMWID, int Enable);
//--------------------------------------------------------------------------------------------
/**
 * Get current status of the Digital I/Q Info Interface. This function can be called from
 * another thread than all the other functions. However, it is not possible to call this
 * function from more than one thread!
 *
 * @param pConnectionState  OUT: Pointer to the connection state of the Digital I/Q Interface:
 *                              -2: Unknown/undefined state: this state shall never occur.
 *                              -1: Error: a protocol error has been detected. The error is
 *                                  internally cleared after calling this method, or when the
 *                                  Digital I/Q cable is disconnected. For most errors, the
 *                                  TSMW will retry the connection procedure, until it is
 *                                  successful.
 *                               0: Disconnected: the Digital I/Q cable is not connected.
 *                               1: Connection in progress: the handshaking procedure between
 *                                  the devices is running.
 *                               2: Reserved for later use.
 *                               3: Ready: the configuration of the Digital I/Q connection has
 *                                  been completed without error.
 * @param pRxChanMuxSupport OUT: Pointer to the capability of the target device to support
 *                               multiple channels:
 *                               0: Single channel only. If the target device is only
 *                                  supporting 1 channel and the TSMW is configured to send
 *                                  multiple channels, only one channel will be transmitted
 *                                  over the Digital I/Q Interface ; the channel transmitted
 *                                  will be the first channel of the first filterbank
 *                                  configured, i.e. channel 1 from filterbank1 or channel 1
 *                                  of filterbank2 (if filterbank1 is not used).
 *                               1: Multiple channels supported.
 * @param pChanMuxMode      OUT: Pointer to the current mode negociated between the devices:
 *                               0: Single channel.
 *                               1: Multiple channels.
 *
 * @return char*           Pointer to Digital I/Q Info Interface status string, valid until
 *                         next call of function
 */
char* __stdcall TSMWINFOIFGetStatus_c (int *pConnectionState, int *pRxChanMuxSupport,
                                       int *pChanMuxMode);
//--------------------------------------------------------------------------------------------
/**
 * Set/read TSMW trigger IOs
 *
 * @param TSMWID           IN: ID of TSMW to use for streaming
 * @param TriggerLine      Trigger line that is addressed
 *                            1: Trigger 1
 *                            2: Trigger 2
 * @param Out              0: Use as input
 *                         1: Use as output
 * @param pTrig1Value      IN/OUT: Set/get current trigger level for trigger line 1 (0: low, 1: high)
 * @param pTrig2Value      IN/OUT: Set/get current trigger level for trigger line 2 (0: low, 1: high)
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWTrigger_c (unsigned short TSMWID, int TriggerLine, int Out,
                             int *pTrig1Value, int *pTrig2Value);









//--------------------------------------------------------------------------------------------
/**
 * Start the TCP server forwarding NMEA sentences over LAN
 *
 * @param TSMWID           IN: ID of TSMW used as NMEA source
 * @param ipPort           IN: TCP listening port of the server, valid range 0 to 65535,
 *                             recommended range 49152 to 65535
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSStartTcpServer_c (unsigned short TSMWID, unsigned short ipPort);

//--------------------------------------------------------------------------------------------
/**
 * Stop the GPS server
 *
 * @param TSMWID           IN: ID of TSMW used as NMEA source
 *
 * @return int             Errorcode, 0 if successful
 */
int __stdcall TSMWGPSStopServer_c (unsigned short TSMWID);

//--------------------------------------------------------------------------------------------
/**
 * Status of the GPS server
 *
 * @param TSMWID           IN: ID of TSMW used as NMEA source
 * @param pStatus          OUT: Status of the GPS server:
 *                               0: Server not running
 *                               1: Server running (idle)
 *                               2: Server running (busy)
 *                               3: Server dead (fatal error)
 * @param pPort            OUT: listening port of the GPS server
 *
 * @return char*           Pointer to error string, valid until next call of this function
 */
char* __stdcall TSMWGPSGetServerStatus_c (unsigned short TSMWID, unsigned char *pStatus, unsigned short *pPort);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // _TSMWIQINTERFACEFUNC_H

