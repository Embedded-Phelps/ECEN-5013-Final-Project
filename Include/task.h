#ifndef __ST_TASK_H__
#define __ST_TASK_H__

#include "includes.h"

#define ST_MSG_QUEUE_SIZE				(16U)

#define ST_KEY_PRESSED_MSG				(1U)

#define ST_PERIODIC_EVT_UNITPERIOD		(1000U)
#define ST_SENSE_TOUCH_EVT				(0x00000001U)
#define ST_READ_TEMP_EVT				(0x00000002U)
#define ST_BLINK_LED_EVT				(0x00000004U)

#if defined(__cplusplus)
extern "C" {
#endif

void ST_TaskFunc(void);

#if defined(__cplusplus)
}
#endif

#endif
