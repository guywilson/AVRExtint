#include <stddef.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "pcint_attiny85.h"

uint16_t	pcIntCount = 0;
uint8_t		pinState = 0xFF;


void setupPCInt()
{
	/*
	** Set pin B4 (PCINT4) as an input...
	*/
	DDRB &= ~_BV(DDB4);

	/*
	** Activate the pull-up resistor...
	*/
	PORTB |= _BV(PORTB4);

	/*
	** Enable PC interrupt...
	*/
	GIMSK |= _BV(PCIE);
	PCMSK |= _BV(PCINT4);
}

void resetPCIntCount()
{
	pcIntCount = 0;
}

uint16_t getPCIntCount()
{
	return pcIntCount;
}

/*
 * PC interrupt routine...
 */
ISR(PCINT0_vect)
{
	if (!pinState) {
		pcIntCount++;
		pinState = !pinState;
	}
}
