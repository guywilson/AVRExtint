#include <avr/io.h>

#include "led_utils.h"

void setupLEDPin(void)
{
	/*
	** Set Port B - Pin 3 as output
	*/
    DDRB |= _BV(DDB3);
}

void turnOn(int LED_ID)
{
	/* set pin 3 high to turn led on */
	PORTB |= LED_ID;
}

void turnOff(int LED_ID)
{
	/* set pin 3 low to turn led off */
	PORTB &= ~LED_ID;
}

void toggleLED(int LED_ID)
{
	static unsigned char state = 0;
	
	if (!state) {
		turnOn(LED_ID);
		state = 1;
	}
	else {
		turnOff(LED_ID);
		state = 0;
	}
}
