#include <stdint.h>
#include <avr/interrupt.h>

#include "rtc_attiny85.h"

//#define ENABLE_RTC_TICK_TASK

volatile uint32_t 			_realTimeClock;

#ifdef ENABLE_RTC_TICK_TASK
static volatile uint16_t	_tickCount = 0;

static void (* _tickTask)();

void nullTick()
{
	// Do nothing...
}
#endif

void setupRTC()
{
    // Clear registers
    TCCR1 = 0x00;
    
    TCNT1  = 0;
    
    /*
    ** Timer value = (clock_speed / (interrupt_freq * pre-scaler)) - 1
    ** Timer value = (16,000,000 / (1000 * 32)) - 1
    */
    OCR1B = __TIMER_COUNT;

    // turn on CTC mode
    TCCR1 |= _BV(CTC1);
    
    // Set 64x prescaler
    TCCR1 |= _BV(CS12) | _BV(CS11) | _BV(CS10);
    
    // enable timer compare interrupt
    TIMSK |= (1 << OCIE1A);

#ifdef ENABLE_RTC_TICK_TASK
    // Register the nullTick() function...
    registerTickTask(&nullTick);
#endif
    _realTimeClock = 0L;
}

uint32_t getCurrentTime()
{
	return _realTimeClock;
}

#ifdef ENABLE_RTC_TICK_TASK
void registerTickTask(void (* tickTask)())
{
	_tickTask = tickTask;
}
#endif

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
#ifdef ENABLE_RTC_TICK_TASK
	_tickCount++;

	if (_tickCount == RTC_INTERRUPT_PRESCALER) {
#endif
		/*
	    ** The RTC is incremented every 1 ms,
		** it is used to drive the real time clock
		** for the scheduler...
	    */
		_realTimeClock++;
#ifdef ENABLE_RTC_TICK_TASK
		_tickCount = 0;
	}

	/*
	 * Run the tick task, defaults to the nullTick() function.
	 *
	 * This must be a very fast operation, as it is outside of
	 * the scheduler's control. Also, there can be only 1 tick task...
	 */
	_tickTask();
#endif
}
