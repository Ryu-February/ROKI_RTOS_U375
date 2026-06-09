/*
 * msg_types.h
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#ifndef IPC_MSG_TYPES_H_
#define IPC_MSG_TYPES_H_

#include "cmsis_os2.h"
#include "mode_sw.h"

typedef enum
{
	UI_EVT_MODE_CHANGED,
	UI_EVT_RUBBER_PRESSED,
	UI_EVT_MODE_NONE,
}ui_evt_type_t;


typedef struct
{
	ui_evt_type_t 	type;
	mode_sw_t 	mode;
	uint8_t 	rubber_id;

//	union
//	{
//		mode_sw_t 	mode;
//		uint8_t 	rubber_id;
//	};
}ui_msg_t;

extern osMessageQueueId_t ui_queue;



#endif /* IPC_MSG_TYPES_H_ */
