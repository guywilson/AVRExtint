#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "sched/scheduler.h"
#include "sched/schederr.h"
#include "taskdef.h"
#include "heartbeat.h"

#include "led_utils.h"
#include "rtc_attiny85.h"
#include "pcint_attiny85.h"
#include "spis_attiny85.h"

void main(void) __attribute__ ((noreturn));

void setup(void)
{
	setupLEDPin();
	setupRTC();
	setupPCInt();
	setupSPISlave();
}

#pragma GCC diagnostic ignored  "-Wmain"
void main(void)
{
	/*
	 * Disable interrupts...
	 */
	cli();

	setup();

	/*
	 * Enable interrupts...
	 */
    sei();
	
	initScheduler();

	registerTask(TASK_HEARTBEAT, &HeartbeatTask);
//	registerTask(TASK_SPI, &SPITask);

	scheduleTask(
			TASK_HEARTBEAT,
			rtc_val_ms(950),
			NULL);

	/*
	** Start the scheduler...
	*/
	schedule();
}
