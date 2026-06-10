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




extern osMessageQueueId_t ui_queue;


osTimerId_t rgb_off_timer_id;

void rgb_off_timer_callback(void *argument)
{
	(void)argument;

	ui_msg_t msg = {.type = UI_EVT_TIMEOUT_OFF};

	osMessageQueuePut(ui_queue, &msg, 0, 0);
}



void ui_task(void *argument)
{
	(void)argument;

	ui_feedback_init();

	rgb_off_timer_id = osTimerNew(rgb_off_timer_callback, osTimerOnce, NULL, NULL);

	for (;;)
	{
		ui_msg_t msg;


		if (osMessageQueueGet(ui_queue, &msg, NULL, osWaitForever) != osOK)
		{
			continue;
		}

		switch (msg.type)
		{
			case UI_EVT_MODE_CHANGED:
				ui_feedback_apply_mode(msg.mode);
				break;
			case UI_EVT_BTN_PRESSED:
				ui_feedback_btn_press_start(msg.btn);

				// ② 기존에 돌던 타이머가 있다면 멈추고, 새로 1초(1000ms) 시작
				osTimerStop(rgb_off_timer_id);
				osTimerStart(rgb_off_timer_id, 500);
				break;
			case UI_EVT_BAT_INDICATE:
				ui_feedback_indicate_battery(msg.bat_low);
				break;
			case UI_EVT_TIMEOUT_OFF:
				ui_feedback_btn_press_timeout();
				break;
			case UI_EVT_NOT_HAPPEND:
				break;

			default:
				break;
		}

//		ui_feedback_set(UI_STATE_BOOT);
//
//		osDelay(500);
	}
}
