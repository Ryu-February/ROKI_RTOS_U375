/*
 * ui_feedback.h
 *
 *  Created on: 2026. 6. 8.
 *      Author: RCY
 */

#ifndef SERVICES_UI_FEEDBACK_UI_FEEDBACK_H_
#define SERVICES_UI_FEEDBACK_UI_FEEDBACK_H_


#include "mode_sw.h"

typedef enum
{
	UI_STATE_BOOT,
	UI_STATE_RUN,
	UI_STATE_ERROR,
}ui_state_t;




void ui_feedback_init(void);
void ui_feedback_set(ui_state_t state);

void ui_feedback_apply_mode(mode_sw_t mode);

#endif /* SERVICES_UI_FEEDBACK_UI_FEEDBACK_H_ */
