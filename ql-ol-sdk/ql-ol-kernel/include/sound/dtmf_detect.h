/* Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __DTMF_DETECT_H__
#define __DTMF_DETECT_H__

#include <linux/types.h>
#include <linux/ioctl.h>

#define MSG_REQUEST  0x1
#define MSG_RESPONSE 0x2

struct dtmf_detect_write_msg {
	__u32 msg_type;
	__u8 payload[0];
};

struct dtmf_detect_register {
	__u8 reg_flag;
};

struct dtmf_detect_cmd_response {
	__u16 low_freq;
	/**< Low frequency detected in the DTMF tone.
			 @values 697, 770, 852, 941 Hz */
	__u16 high_freq;
	/**< High frequency detected in the DTMF tone.
			 @values 1209, 1336, 1477, 1633 Hz */
};

struct dtmf_detect_cmd_request {
	__u32 payload_size;
	__u8 payload[0];
};

int dtmf_detection_event_process(struct dtmf_detect_cmd_response);
#endif
