/*
 * ui_task.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#include "ui_task.h"
#include "cmsis_os2.h"

#include "ui_feedback.h"
#include "msg_types.h"


volatile uint32_t task_hit_count = 0;

extern osMessageQueueId_t ui_queue;

void ui_task(void *argument)
{
	(void)argument;

	ui_feedback_init();

	for (;;)
	{
		ui_msg_t msg;


		if (osMessageQueueGet(ui_queue, &msg, NULL, osWaitForever) != osOK)
		{
			continue;
		}

		task_hit_count++;

		switch (msg.type)
		{
			case UI_EVT_MODE_CHANGED:
				ui_feedback_apply_mode(msg.mode);
				break;
			case UI_EVT_RUBBER_PRESSED:
				/*구현*/
				break;
			case UI_EVT_MODE_NONE:
				break;
//			msg.type = UI_
		}

//		ui_feedback_set(UI_STATE_BOOT);
//
//		osDelay(500);
	}
}
