/*
 * ql_voice_record.h 
 *
 * Author: Grady <grady.quan@quectel.com>
 *
 * Date: 2018-03-27
 */

#ifndef __QL_VOICE_RECORD_H__
#define __QL_VOICE_RECORD_H__

/*
* Function:     device path set
* 
* Description:
*               set the uplink or downlink device path
*
* Parameters:
*               uplink or downlink 
* Return:        
*               TURE or FALSE
*/
int ql_voice_record_dev_set(int dlink);

/*
* Function:     device path clear
* 
* Description:
*               clear the uplink or downlink device path
*
* Parameters:
*               uplink or downlink 
* Return:        
*               TURE or FALSE
*/
int ql_voice_record_dev_clear(int dlink);

/*
* Function:     open the pcm device 
* 
* Description:
*               open the pcm device
*
* Parameters:
*               rate :sampling rate, 8000 or 16000
*               channel :mono/stereo, 1 or 2
* Return:        
*               TURE or FALSE
*/
int ql_voice_record_open(unsigned int rate, unsigned int channels);

/*
* Function:     get pcm buffer lenth
* 
* Description:
*               get pcm buffer lenth
*
* Parameters:
*               none
* Return:        
*               pcm buffer lenth
*/
int ql_get_voice_record_buffer_len(void);

/*
* Function:     record data read 
* 
* Description:
*               read the record data
*
* Parameters:
*               the input buffer point
* Return:        
*               read pcm data lenth
*/
int ql_voice_record_read(char *buffer);

/*
* Function:     close the pcm device 
* 
* Description:
*               close the pcm device
*
* Parameters:
*               none
* Return:        
*               TURE or FALSE
*/
int ql_voice_record_close(void);



#endif  //__QL_VOICE_RECORD_H__
