/*
 * ui_feedback.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */



#include "ui_feedback.h"
#include "led.h"
#include "rgb.h"
#include "buzzer.h"




void ui_feedback_init(void)
{
	led_init();
	rgb_init();
}

static color_t color_by_button(btn_id_t id)
{
    switch (id)
    {
        case BTN_FORWARD:  return COLOR_BLUE;    // 전진 = 파랑
        case BTN_BACKWARD: return COLOR_RED;     // 후진 = 빨강
        case BTN_LEFT:     return COLOR_PURPLE;  // 좌회전 = 보라
        case BTN_RIGHT:    return COLOR_YELLOW;  // 우회전 = 노랑
        case BTN_EXECUTE:  return COLOR_GREEN;   // GO = 초록
        case BTN_DELETE:   return COLOR_WHITE;    // DELETE = 하양
        case BTN_RESUME:   return COLOR_WHITE;  // RESUME = 하양
        default:           return COLOR_BLACK;
    }
}

// 💡 [개선] 버튼에 따른 부저음 재생 로직을 별도 함수로 분리
static void buzzer_by_button(btn_id_t id)
{
    switch (id)
    {
        case BTN_EXECUTE: buzzer_play_execute();       break;
        case BTN_DELETE:  buzzer_evt_delete();         break;
        case BTN_RESUME:  buzzer_play_resume();        break;
        default:          buzzer_play_dir_click_soft(); break;
    }
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

void ui_feedback_indicate_battery(uint8_t low)
{
	if (low)
	{
		led_on(LED_POWER_STAT_O);
		led_off(LED_POWER_STAT_W);
	}
	else
	{
		led_on(LED_POWER_STAT_W);
		led_off(LED_POWER_STAT_O);
	}
}

void ui_feedback_btn_press_start(btn_id_t btn)
{
	// 1. LED 피드백
	color_t btn_to_col = color_by_button(btn);
	rgb_set_color(RGB_ZONE_V_SHAPE, btn_to_col);

	// 2. 부저 피드백 (분리한 함수 호출로 단 한 줄로 정리!)
	buzzer_by_button(btn);
}

void ui_feedback_btn_press_timeout(void)
{
	rgb_set_color(RGB_ZONE_V_SHAPE, COLOR_BLACK);
}
