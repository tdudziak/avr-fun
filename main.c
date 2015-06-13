#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "debug.h"

int main(void) __attribute__((noreturn));

const uint16_t BLINKER_HZ = 1;
const float BLINKER_DUTY_CYCLE = 0.7;

/**
 * Blinks PORTB1 with configurable duty cycle. Uses hardware PWM.
 */
int main(void)
{
	cli();

	/* set port B to output */
	DDRB = 0xff;

	/*
	 * Timer/Counter1 (the 16-bit one) setup: prescaler set to 1024,
	 * non-inverting "Fast PWM" mode
	 */
	TCCR1A = _BV(WGM11) | _BV(COM1A1);
	TCCR1B = _BV(WGM13) | _BV(CS12) | _BV(CS10);
	ICR1 = F_CPU/1024/BLINKER_HZ;
	OCR1A = BLINKER_DUTY_CYCLE*ICR1;

	for (;;) {
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
