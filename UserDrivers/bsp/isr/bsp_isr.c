/*
 * bsp_isr.c
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */


#include "bsp_isr.h"

#include "def.h"
#include "rgb.h"
#include "buzzer.h"

volatile uint32_t timer6_ms;

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;


void bsp_isr_init(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}

void bsp_rgb_timer_start(void)
{
	HAL_TIM_Base_Start_IT(&htim7);
}



void bsp_isr_tim4_callback(void)	//30us timer
{

}

void bsp_isr_tim6_callback(void)	//1ms_timer
{
	timer6_ms++;
	buzzer_update_1ms();
}

void bsp_isr_tim7_callback(void)	//60us timer
{
	rgb_tick();
}
