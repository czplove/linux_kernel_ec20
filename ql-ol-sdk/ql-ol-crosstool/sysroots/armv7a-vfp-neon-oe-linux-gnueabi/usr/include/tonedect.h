
/*****************************************************************************
 *
 * Filename:
 * ---------
 * tonedect.h
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 * header of tone detection function
 *
 * Author:
 * -------
 * -------
 * -------
 *============================================================================
 *             HISTORY

 ****************************************************************************/
#ifndef __TONEDECT_H__
#define __TONEDECT_H__

#ifndef int8
#define int8  signed char 
#endif
#ifndef int16
#define int16  signed short
#endif
#ifndef int32
#define int32  signed long
#endif
#ifndef uint8
#define uint8  unsigned char 
#endif
#ifndef uint16
#define uint16  unsigned short 
#endif
#ifndef uint32
#define uint32  unsigned long 
#endif
#ifndef boolean
#define boolean  unsigned char 
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define SAMPLING_RATE      8000     /* 8000Hz   */

typedef struct {
   int32 coeff;
   int32 Q1;
   int32 Q2;
} TD_Handle;

/*
* {victor} dtmfHandle : 
block,mag,key is data used in dtmf decoder process,and other some methods.
*
*/
typedef struct dtmfHandle DTMF_Handle;

struct dtmfHandle 
{
  int32       block_size;											//size of the DTMF analysis block
  int32       acum_buf_size;										//
  int32       block_no;												//number of the block
  TD_Handle   hdl[14];
  int32       block_count;
  int32       mag[14];
  TD_Handle   hdl2[14];
  int32       block_count2;
  int32       mag2[14];
  uint32      key_ebb;
  uint32      kay_strong;
  uint32      pause_f7;
  uint32      pause_dtmf;
  uint32      pause_dtmfunkwon;
  int16       key;
  int16       key_temp;
  int8        pause_type;
  uint16      framecnt ;
  int32       (*PutData)( DTMF_Handle *hdl, int16 *buf ,int32 len);	//return value: consume length of input buffer
  void        (*Close)( DTMF_Handle *hdl );
  int16       (*GetKey)( DTMF_Handle *hdl );
};

void TD_Init( TD_Handle *hdl, int32 target_freq, int32 block_size );
void TD_Reset( TD_Handle *hdl );
void TD_ProcessSample( TD_Handle *hdl, int16 sample );
void TD_ProcessBlock( TD_Handle *hdl, int16 *buf, int32 len );
int32 TD_GetMagnitudeDB( TD_Handle *hdl );
DTMF_Handle *DTMF_Open( int32 block_size, int32 block_no );

/*ivan 20150630 add 增大dtmfdata缓冲区，避免数据丢失问题*/
#define  FRAME_BUF_NO         4
#define  FRAME_BUF_MASK       (FRAME_BUF_NO-1)
#define  FRAME_BUF_SIZE       160
#define  DTMF_FRAME_SIZE      106

#define  IDLE_STATE           0
#define  SPEECH_STATE         1
#define  VOIP_STATE           2

#endif /* __TONEDECT_H__ */

