#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "debug.h"

int main(void) __attribute__((noreturn));

int main(void)
{
	// disable interrupts
	cli();

	// set pin 5 of PORTB for output
	DDRB |= _BV(DDB5);

	// interrupt every 1s
	OCR1A = F_CPU/1024;

	// mode 4, CTC on OCR1A
	TCCR1B |= (1 << WGM12);

	// set interrupt on compare match
	TIMSK1 |= (1 << OCIE1A);

	// set prescaler to 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// enable interrupts
	sei();

	for (;;) {}
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t state = 0;
	state = !state;

	if (state) {
		PORTB |= _BV(PORTB5);
		dprintf("Hello, ");
	} else {
		PORTB &= ~_BV(PORTB5);
		dprintf("world!\n");
	}
}
