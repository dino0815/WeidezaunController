/************************************************************************/
/*                                                                      */
/*                      RC5 Remote Receiver                             */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define uchar unsigned char
#define uint unsigned int

#define	BAUD	19200
#define bauddivider (uint)(F_CPU / BAUD / 16 - 0.5)

extern uint	rc5_data;				// store result

void uart_put_char( char c ){
  while( (UCSRA & 1<<UDRE) == 0 );
  UDR = c;
}


void uart_put_string( char *s ){
  while( *s )
    uart_put_char( *s++ );
}


int main( void ){
  uint i;
  char s[30];

  TCCR0 = 1<<CS02;			//divide by 256
  TIMSK = 1<<TOIE0;			//enable timer interrupt

  UBRRL = bauddivider;			//set baud rate
  UBRRH = bauddivider >> 8;
  UCSRA = 0;				//no U2X, MPCM
  UCSRC = 1<<URSEL^1<<UCSZ1^1<<UCSZ0;	//8 Bit
  UCSRB = 1<<RXEN^1<<TXEN;		//enable RX, TX

  sei();
  uart_put_string( "RC5-Dekoder:\n\r" );
  for(;;){				// main loop
    cli();
    i = rc5_data;			// read two bytes from interrupt !
    rc5_data = 0;
    sei();
    if( i ){
      DDRB = i;				// LED output
      uart_put_char(( i >> 11 & 1) + '0');	// Toggle Bit
      uart_put_char(' ');
      itoa( i >> 6 & 0x1F, s, 10);	// Device address
      uart_put_string( s );
      uart_put_char(' ');
      itoa((i & 0x3F) | (~i >> 7 & 0x40), s, 10); // Key Code
      uart_put_string( s );
      uart_put_string( "\n\r" );
    }
  }
}
