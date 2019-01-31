/*
 * ql_acdb.h 
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#ifndef __QL_ACDB_H__
#define __QL_ACDB_H__

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif
//------------------------------------------------------------------------------
/*
* Function:     ql_audplay_gain_write
* 
* Description:
*               set the audio play gain module in audio playback 
*
* Parameters:
*               the audio play gain value, range:0~65535 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_audplay_gain_write(unsigned short gain);

//------------------------------------------------------------------------------
/*
* Function:     ql_audplay_gain_read
* 
* Description:
*               read the audio play gain module in audio playback 
*
* Parameters:
*               none. 
* Return:        
*               the audplay gain(0~65535). 
*		the value is -1 when return error.
*/
//------------------------------------------------------------------------------
int ql_audplay_gain_read(void);

//------------------------------------------------------------------------------
/*
* Function:     ql_audrd_gain_write
* 
* Description:
*               set the audio record gain module in audio record 
*
* Parameters:
*               the audio record gain value, range:0~65535 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_audrd_gain_write(unsigned short gain);

//------------------------------------------------------------------------------
/*
* Function:     ql_audrd_gain_read
* 
* Description:
*               read the audio record gain module in audio record 
*
* Parameters:
*               none. 
* Return:        
*               the audio record gain(0~65535). 
*		the value is -1 when return error.
*/
//------------------------------------------------------------------------------
int ql_audrd_gain_read(void);

//------------------------------------------------------------------------------
/*
* Function:     audloop read
* 
* Description:
*               read the audloop value
*
* Parameters:
*               none. 
* Return:        
*               the audloop value: enable or disable
*/
//------------------------------------------------------------------------------
int ql_audloop_read(void);

//------------------------------------------------------------------------------
/*
* Function:     audloop write
* 
* Description:
*               set the audloop value
*
* Parameters:
*               enable or disable, range:0~1 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_audloop_write(unsigned int value);

//------------------------------------------------------------------------------
/*
* Function:     clvl read
* 
* Description:
*               read the clvl value
*
* Parameters:
*               none. 
* Return:        
*               the clvl value
*/
//------------------------------------------------------------------------------
int ql_clvl_read(void);

//------------------------------------------------------------------------------
/*
* Function:     clvl write
* 
* Description:
*               set the clvl value, range:0~5
*
* Parameters:
*               clvl value 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_clvl_write(unsigned int value);

/*****************************qmic api********************************/
//------------------------------------------------------------------------------
/*
* Function:     ql_mic_gain_write
* 
* Description:
*               set the mic gain to change uplink volume
*
* Parameters:
*               vol     :voice volume, range:0~65535
*               mic_gain:the mic gain value, range:0~65535
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_mic_gain_write(unsigned short vol, unsigned short mic_gain);

//------------------------------------------------------------------------------
/*
* Function:     ql_mic_gain_read
* 
* Description:
*               read the mic gain and vol
*
* Parameters:
*               vol     :the point of input vol. 
*               mic_gain:the point of input mic_gain. 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_mic_gain_read(unsigned short *vol, unsigned short *mic_gain);

/*****************************qspk api********************************/
//------------------------------------------------------------------------------
/*
* Function:     ql_spk_gain_write
* 
* Description:
*               set the spk gain to change downlink volume
*
* Parameters:
*               spk_gain:the downlink gain value, range:0~65535
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_spk_gain_write(unsigned short spk_gain);

//------------------------------------------------------------------------------
/*
* Function:     ql_spk_gain_read
* 
* Description:
*               read the downlink volume
*
* Parameters:
*               spk_gain:the point of input downlink volume. 
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_spk_gain_read(unsigned short *spk_gain);

/*******************************qsidet api************************************/
//------------------------------------------------------------------------------
/*
* Function:     qsidet read
* 
* Description:
*               read the sidetone value
*
* Parameters:
*               none. 
* Return:        
*               the sidetone value, range:0~65535
*/
//------------------------------------------------------------------------------
int ql_sidet_read(void);

//------------------------------------------------------------------------------
/*
* Function:     qsidet write
* 
* Description:
*               set the sidetone value
*
* Parameters:
*               the sidetone value, range:0~65535
* Return:        
*               TRUE or FALSE. 
*/
//------------------------------------------------------------------------------
int ql_sidet_write(unsigned int value);


#endif  //__QL_ACDB_H__
