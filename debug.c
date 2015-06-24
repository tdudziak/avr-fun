#include "debug.h"

#include <avr/io.h>
#include <util/setbaud.h>

#ifndef NDEBUG
static int uart_putchar(char c, FILE* stream)
{
	/* wait until data register is ready */
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;

	/* follow \n with \r since that's what a serial terminal expects */
	if (c == '\n')
		uart_putchar('\r', stream);

	return 0;
}

static FILE* uart_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* enable RX and TX */

    static FILE uart_stream =
	    FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

    /* safe to return pointer to a static local */
    return &uart_stream;
}

FILE* debug_get_stream()
{
	static FILE* debug_stream = NULL;

	/*
	 * Race condition: we might run uart_init multiple times if
	 * this is called from an interrupt. Not really an issue.
	 */
	if (debug_stream == NULL)
		debug_stream = uart_init();

	return debug_stream;
}
#endif
