#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "debug.h"

int main(void) __attribute__((noreturn));

const uint8_t BLINK_HZ = 3;

/**
 * Toggles output pin B1 BLINK_HZ times per second.
 *
 * Done by the built-in timer which can be configured to toggle pin output
 * on compare match.
 */
int main(void)
{
	cli();

	/* set port B to output */
	DDRB = 0xff;

	/* Timer/Counter1 (the 16-bit one) setup */
	TCCR1B = _BV(WGM12) /* clear timer on compare match mode */
	       | _BV(CS12) | _BV(CS10); /* set prescaler to 1024 */
	TCCR1A = _BV(COM1A0); /* toggle output on compare match */
	OCR1A = F_CPU/1024/BLINK_HZ;

	for (;;) {
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}
