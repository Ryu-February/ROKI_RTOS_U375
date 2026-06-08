/*
 * app.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#include "app.h"
#include "cmsis_os2.h"
#include "ui_task.h"



osThreadAttr_t comm_attr =
{
	.stack_size = 128 * 4,
	.priority = osPriorityLow,
};


void app_init(void)
{
	osThreadNew(ui_task, NULL, &comm_attr);
}


