#ifndef __ST_TASK_H__
#define __ST_TASK_H__

#include "includes.h"

#define ST_MSG_QUEUE_SIZE				(16U)

#define ST_KEY_PRESSED_MSG				(1U)

#define ST_PERIODIC_EVT_UNITPERIOD		(1000U)
#define ST_SENSE_TOUCH_EVT				(0x00000001U)
#define ST_READ_TEMP_EVT				(0x00000002U)
#define ST_BLINK_LED_EVT				(0x00000004U)

#define SYSTEM_INITIALIZED	(0x0)
#define SYSTEM_ENTERED_WAIT (0x1)
#define SYSTEM_NORMAL				(0x2)
#define SYSTEM_FAILED				(0x3)
#define READING_SYS_TEMPERATURE	(0x10)
#define SYSTEM_TEMPERATURE_NORMAL (0x11)
#define SYSTEM_TEMPERATURE_OUTOFRANGE (0x12)
#define SYSTEM_SENSED_TOUCH	(0x20)
#if defined(__cplusplus)
extern "C" {
#endif

void ST_TaskFunc(void);

#if defined(__cplusplus)
}
#endif

#endif
