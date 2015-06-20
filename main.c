#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "debug.h"

int main(void) __attribute__((noreturn));

/**
 * A pointless waste of memory but whatever...
 */
uint8_t PATTERNS[] = {
	/* go right to left */
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	/* go left to right */
	_BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0),
	/* start in the center and expand */
	0x18, 0x3c, 0x7e, 0xff, 0xff, 0xff,
	/* blink like crazy */
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
};

/**
 * There is an 8-bit M74HC595B1 shift register connected as follows:
 *
 *     PORTB3 (MOSI)  <->   SI
 *     PORTB5 (SCK)   <->   SCK
 *     PORTB2 (SS)    <->   RCK
 *
 * The program communicates with the shift register by using the SPI hardware
 * and does some blinking on the LEDs that are connected to the register.
 */
int main(void)
{
	cli();

	/* set port B to output */
	DDRB = 0xff;

	/* enable and configure SPI interface */
	SPCR = _BV(SPE) | _BV(DORD) | _BV(MSTR) | _BV(SPR1);
	SPDR = 0x00;

	for (unsigned i = 0;; i++) {
		PORTB ^= _BV(PORTB2);
		SPDR = PATTERNS[i % sizeof(PATTERNS)];
		_delay_ms(100);
		PORTB ^= _BV(PORTB2);
		_delay_ms(10);
	}
}
