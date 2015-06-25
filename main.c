#include <atom.h>
#include <atomport-private.h>

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "debug.h"

static ATOM_TCB main_tcb;

static uint8_t main_thread_stack[256];
static uint8_t idle_thread_stack[128];

static void main_thread_func(uint32_t data)
{
	/* set port B to output */
	DDRB = 0xff;

	for (;;) {
		/* blink the Arduino LED */
		PORTB ^= _BV(PORTB5);
		_delay_ms(500);
	}
}

int main(void)
{
	int8_t status;
	dprintf("initializing...\n");

	/* TODO: implement panics; check for ATOM_OK */
	status = atomOSInit(
		&idle_thread_stack[0],
		sizeof(idle_thread_stack)/sizeof(idle_thread_stack[0]),
		FALSE);

	avrInitSystemTickTimer();

	status = atomThreadCreate(
		&main_tcb,
		0,
		main_thread_func,
		0,
		&main_thread_stack[0],
		sizeof(main_thread_stack)/sizeof(main_thread_stack[0]),
		TRUE);

	atomOSStart();
	return 0;
}
