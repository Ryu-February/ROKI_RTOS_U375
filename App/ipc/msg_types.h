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
#include "btn.h"

typedef enum
{
	UI_EVT_MODE_CHANGED,
	UI_EVT_BTN_PRESSED,
	UI_EVT_BAT_INDICATE,
	UI_EVT_TIMEOUT_OFF,
	UI_EVT_NOT_HAPPEND,
}ui_evt_type_t;


typedef struct
{
	ui_evt_type_t 	type;
	mode_sw_t 		mode;
	btn_id_t 		btn;
	bool			bat_low;
}ui_msg_t;

typedef struct
{
	uint16_t		ir;
}control_msg_t;

extern osMessageQueueId_t ui_queue;



#endif /* IPC_MSG_TYPES_H_ */
