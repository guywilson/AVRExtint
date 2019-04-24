#include <stddef.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "rtc_attiny85.h"
#include "spis_attiny85.h"
#include "pcint_attiny85.h"

volatile uint16_t		pulseCount;

void setupSPISlave()
{
	DDRB |= _BV(DDB1);		// MISO is an output
	DDRB &= ~_BV(DDB0);		// MOSI is an input
	DDRB &= ~_BV(DDB2);		// SCK is an input

	/*
	 * On the ATTiny85, we use the USI port as an SPI.
	 *
	 * Enable 3-wire mode
	 * External clock (e.g. SCK from the master)
	 * Enable the USI counter overflow interrupt.
	 */
	USICR = _BV(USIWM0) | _BV(USICS1) | _BV(USIOIE);
}

ISR(USI_OVF_vect)
{
	uint8_t 	spiCmd = USIBR;

	switch (spiCmd) {
		case SPI_CMD_GET_COUNT_LSB:
			pulseCount = getPCIntCount();
			USIDR = (uint8_t)(pulseCount & 0x00FF);
			break;

		case SPI_CMD_GET_COUNT_MSB:
			USIDR = (uint8_t)((pulseCount >> 8) & 0x00FF);
			break;

		case SPI_CMD_RESET_COUNTER:
			resetPCIntCount();
			USIDR = 'A';
			break;
	}
}
