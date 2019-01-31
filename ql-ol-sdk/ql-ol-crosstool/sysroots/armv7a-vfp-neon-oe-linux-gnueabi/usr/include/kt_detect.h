
#ifndef __KT_DETECT_H__
#define __KT_DETECT_H__

#include <stdio.h>
#include <stdarg.h>
#include "tonedect.h"

typedef struct
{
  int16         *rb_base;					//ring buffer address
  uint16         aud_id;
  uint32          tmp_r;
  uint32          tmp_w;
  DTMF_Handle   *hdl;
  void           (*Callback)( int16 key );
  uint16          state;
  uint8           stage;
  uint8           enable;
} type_kt_detect;

void DTMFDetectInit(void);
void KtDetectStart(void );

void KtDetectStop( void );
void ktDetectDedi( void );

boolean KtDetectBeStarted(void);
void KtDetectSetState(boolean started);
void KtDetectCopyPCMData(const uint16 *pcm_data);

#define KT_DETECT_DEBUG 0

#endif /* __KT_DETECT_H__ */

