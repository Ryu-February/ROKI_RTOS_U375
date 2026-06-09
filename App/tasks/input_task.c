/*
 * input_task.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */


#include "input_task.h"

#include "btn.h"
#include "mode_sw.h"
#include "msg_types.h"

#include "cmsis_os2.h"



void input_task(void *argument)
{
    (void)argument;

    mode_sw_init();

    /* 1) 부팅 모드를 1회 강제 발행 (전달 보장 위해 블로킹 put) */
    {
        ui_msg_t msg = { .type = UI_EVT_MODE_CHANGED, .mode = mode_sw_get() };
        osMessageQueuePut(ui_queue, &msg, 0, osWaitForever);
    }

    uint32_t tick = osKernelGetTickCount();
    for (;;)
    {
        mode_sw_update_1ms();
        btn_update_1ms();

        mode_sw_t m = MODE_COUNT;
        if (mode_sw_changed(&m))
        {
            ui_msg_t msg1 = { .type = UI_EVT_MODE_CHANGED, .mode = m };
            /* 2) 전달 보장: 실패로 edge 유실 방지 (짧은 대기/블로킹) */
            osMessageQueuePut(ui_queue, &msg1, 0, osWaitForever);
        }

        btn_id_t b = BTN_COUNT;
        if (btn_pop_any_press(&b))
        {
        	ui_msg_t msg2 = { .type = UI_EVT_BTN_PRESSED, .btn = b };
        	osMessageQueuePut(ui_queue, &msg2, 0, osWaitForever);
        }

        tick += 1;
        osDelayUntil(tick);
    }
}
