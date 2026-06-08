/*
 * bsp_isr.h
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#ifndef BSP_ISR_BSP_ISR_H_
#define BSP_ISR_BSP_ISR_H_



void bsp_isr_init(void);
void bsp_rgb_timer_start(void);



void bsp_isr_tim4_callback(void);
void bsp_isr_tim6_callback(void);
void bsp_isr_tim7_callback(void);




#endif /* BSP_ISR_BSP_ISR_H_ */
