/*
 * control_task.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#include "control_task.h"

#include "mode_sw.h"
#include "rgb.h"
#include "cmsis_os2.h"
#include "msg_types.h"


extern osMessageQueueId_t control_sensor_queue;

volatile uint16_t s_ir = 0;


//PID 제어 및 모터 센싱

void control_task(void *argument)
{
	(void)argument;

	uint32_t tick = osKernelGetTickCount();

	for (;;)
	{
		mode_sw_t current_mode = mode_sw_get();

		switch (current_mode)
		{
			case MODE_BUTTON:
//				rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_WHITE);
				break;
			case MODE_CARD:
				control_msg_t sensor_msg;
				osMessageQueueGet(control_sensor_queue, &sensor_msg, 0, 0);

				s_ir = sensor_msg.ir;

				if (sensor_msg.ir >= 100)
				{
					rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_WHITE);
				}
				else
				{
					rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_ORANGE);
				}
				break;
			case MODE_LINE_TRACING:
//				rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_ORANGE);
				break;
			default:
				break;
		}

		tick += 10;
		osDelayUntil(tick);
	}
}
