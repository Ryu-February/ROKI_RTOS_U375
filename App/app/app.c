/*
 * app.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#include "app.h"
#include "cmsis_os2.h"
#include "ui_task.h"
#include "input_task.h"

#include "msg_types.h"



osThreadAttr_t ui_attr =
{
	.name = "ui_task",
	.stack_size = 128 * 4,
	.priority = osPriorityLow,
};

osThreadAttr_t input_attr =
{
	.name = "input_task",
	.stack_size = 128 * 4,
	.priority = osPriorityNormal,
};


osMessageQueueId_t ui_queue;

void app_init(void)
{
	/* 1) ipc 먼저 생성 (태스크가 시작되자마자 큐를 사용) */
	ui_queue = osMessageQueueNew(8, sizeof(ui_msg_t), NULL);

	/* 2) 태스크 생성 */
	osThreadNew(ui_task,    NULL, &ui_attr);
	osThreadNew(input_task, NULL, &input_attr);
}


