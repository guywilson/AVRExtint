#include <stddef.h>
#include <avr/io.h>

#include "rtc_attiny85.h"
#include "sched/scheduler.h"
#include "heartbeat.h"
#include "led_utils.h"
#include "taskdef.h"

void HeartbeatTask(PTASKPARM p)
{
	static uint8_t on = 0;
	
    if (on) {
		/* set pin 3 low to turn led off */
		PORTB &= ~LED_PORT3;
        on = 0;
		scheduleTask(TASK_HEARTBEAT, rtc_val_ms(950), NULL);
    }
    else {
		/* set pin 3 high to turn led on */
		PORTB |= LED_PORT3;
        on = 1;
		scheduleTask(TASK_HEARTBEAT, rtc_val_ms(50), NULL);
    }
}
