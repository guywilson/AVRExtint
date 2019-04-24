#include <stddef.h>
#include <avr/io.h>

#include "sched/scheduler.h"
#include "taskdef.h"
#include "spitask.h"
#include "rtc_attiny85.h"
#include "pcint_attiny85.h"
#include "spis_attiny85.h"

void SPITask(PTASKPARM p)
{
	uint16_t	pulseCount;

	pulseCount = getPCIntCount();

	/*
	 * Send the LSB first, then the MSB...
	 */
	spiSend((uint8_t)(pulseCount & 0x00FF));
}
