/*
 * ql_acdb.h 
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#ifndef __QL_ACDB_H__
#define __QL_ACDB_H__

//------------------------------------------------------------------------------
/*
* Function:     ql_audplay_gain_write
* 
* Description:
*               set the audio play gain module in audio playback 
*
* Parameters:
*               the audio play gain value. 
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
*               the audio record gain value. 
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

#endif  //__QL_ACDB_H__
