#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "debug.h"

int main(void) __attribute__((noreturn));

int main(void)
{
	cli();

	/* set port B to output */
	DDRB = 0xff;

	/* Timer/Counter1 (the 16-bit one) setup */
	const uint16_t TIMER1_HZ = 3;
	TCCR1B =  _BV(WGM12) /* clear timer on compare match mode */
		| _BV(CS12) | _BV(CS10); /* set prescaler to 1024 */
	OCR1A = F_CPU/1024/TIMER1_HZ;
	TIMSK1 |= (1 << OCIE1A); /* enable timer interrupt */

	for (;;) {
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t state = 0;
	state = 1337*state + 7;
	PORTB = 0;

	switch (state % 3) {
	case 0:
		PORTB |= _BV(PORTB3);
		break;

	case 1:
		PORTB |= _BV(PORTB4);
		break;

	case 2:
		PORTB |= _BV(PORTB5);
		break;
	}

	dprintf("PORTB = %x\n", PORTB);
}
