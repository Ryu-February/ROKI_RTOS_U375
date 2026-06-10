/*
 * sensor_task.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */


#include "sensor_task.h"


#include "msg_types.h"
#include "ir.h"
#include "vbat_adc.h"
#include "battery_monitor.h"



extern osMessageQueueId_t ui_queue;
extern osMessageQueueId_t control_sensor_queue;


#define SENSOR_TASK_PERIOD_MS	20U
#define BATTERY_EVAL_DIV		100U


void sensor_task(void *argument)
{
	(void)argument;


	uint32_t tick = osKernelGetTickCount();
	uint32_t bat_div = 0;


	for (;;)
	{
		if (++bat_div >= BATTERY_EVAL_DIV) // 2000ms
		{
			battery_state_t s_batt;
			battery_result_t res;

			bool running = false;
			uint16_t adc = vbat_read_adc();

			battery_monitor_update(&s_batt, adc, running, &res);

			ui_msg_t vbat_msg;
			vbat_msg.type 	= UI_EVT_BAT_INDICATE;
			vbat_msg.bat_low = res.low;

			osMessageQueuePut(ui_queue, &vbat_msg, 0, 0);
		}

		tick += SENSOR_TASK_PERIOD_MS;
		osDelayUntil(tick);
	}
}
