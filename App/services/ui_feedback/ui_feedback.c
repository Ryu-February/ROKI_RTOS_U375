/*
 * ui_feedback.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */



#include "ui_feedback.h"
#include "led.h"
#include "rgb.h"




void ui_feedback_init(void)
{
	led_init();
	rgb_init();
}


void ui_feedback_set(ui_state_t state)
{
//	static color_t c1 = COLOR_GRAY;

	switch (state)
	{
		case UI_STATE_BOOT:
//			if (c1 > COLOR_BLACK)
//				c1 = COLOR_GRAY;
//			rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_BLACK);
//			led_on(LED_W_CONTROL);
			break;
		case UI_STATE_RUN:
			led_toggle(LED_POWER_STAT_W);
			break;
		case UI_STATE_ERROR:
			led_toggle(LED_POWER_STAT_O);
			break;
	}
}

void ui_feedback_apply_mode(mode_sw_t mode)
{
	(mode == MODE_BUTTON) ? led_off(LED_W_CONTROL) : led_on(LED_W_CONTROL);
}
