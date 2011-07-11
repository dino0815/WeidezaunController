/************************************************************************/
/*                                                                      */
/************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define uchar unsigned char
#define uint unsigned int

#define	BAUD	19200
#define bauddivider (uint)(F_CPU / BAUD / 16 - 0.5)

#define	xRC5_IN		PIND
#define	xRC5		PD3			// IR input low active

#define RC5TIME 	1.778e-3		// 1.778msec
#define PULSE_MIN	(uchar)(F_CPU / 512 * RC5TIME * 0.4 + 0.5)
#define PULSE_1_2	(uchar)(F_CPU / 512 * RC5TIME * 0.8 + 0.5)
#define PULSE_MAX	(uchar)(F_CPU / 512 * RC5TIME * 1.2 + 0.5)

uchar	rc5_bit;				// bit value
uchar	rc5_time;				// count bit time
uint	rc5_tmp;				// shift bits in
uint	rc5_data;				// store result

//////////////////////////////////////////////////////////////////////////7
void uart_put_char( char c ){
  while( (UCSRA & 1<<UDRE) == 0 );
  UDR = c;
}

//////////////////////////////////////////////////////////////////////////7
void uart_put_string( char *s ){
  while( *s )
    uart_put_char( *s++ );
}

//////////////////////////////////////////////////////////////////////////7
SIGNAL (SIG_OVERFLOW0){
  uint tmp = rc5_tmp;				// for faster access

  TCNT0 = -2;					// 2 * 256 = 512 cycle

  if( ++rc5_time > PULSE_MAX ){			// count pulse time
    if( !(tmp & 0x4000) && tmp & 0x2000 )	// only if 14 bits received
      rc5_data = tmp;
    tmp = 0;
  }

  if( (rc5_bit ^ xRC5_IN) & 1<<xRC5 ){		// change detect
    rc5_bit = ~rc5_bit;				// 0x00 -> 0xFF -> 0x00

    if( rc5_time < PULSE_MIN )			// to short
      tmp = 0;

    if( !tmp || rc5_time > PULSE_1_2 ){		// start or long pulse time
      if( !(tmp & 0x4000) )			// not to many bits
        tmp <<= 1;				// shift
      if( !(rc5_bit & 1<<xRC5) )		// inverted bit
        tmp |= 1;				// insert new bit
      rc5_time = 0;				// count next pulse time
    }
  }

  rc5_tmp = tmp;
}

//////////////////////////////////////////////////////////////////////////7
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
