/*
 * ui_task.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#include "ui_task.h"
#include "cmsis_os2.h"

#include "ui_feedback.h"




void ui_task(void *argument)
{
	(void)argument;

	ui_feedback_init();

	for (;;)
	{
		ui_feedback_set(UI_STATE_BOOT);

		osDelay(500);
//		osDelay(500);
	}
}
