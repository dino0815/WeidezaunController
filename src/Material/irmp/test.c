/************************************************************************/
/*                                                                      */
/************************************************************************/
 
#include <stdlib.h>

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "irmp.h"
#include "irmpconfig.h"

#define uchar unsigned char
#define uint unsigned int

//#define IRMP_LOGGING  1

#ifndef F_CPU
    #error F_CPU unkown
#endif

#define	BAUD	19200
#define bauddivider (uint)(F_CPU / BAUD / 16 - 0.5)

char s[30];

//////////////////////////////////////////////////////////////////////////7
void uart_put_char( char c ){
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  while( (UCSRA & 1<<UDRE) == 0 );
  UDR = c;
#else
  while( (UCSR0A & 1<<UDRE0) == 0 );
  UDR0 = c;
#endif  // __AVR...
}

//////////////////////////////////////////////////////////////////////////7
void uart_put_string( char *s ){
  while( *s )
    uart_put_char( *s++ );
}

//////////////////////////////////////////////////////////////////////////7
void uart_put_hex( uchar b ){
/*  uart_put_char('0');
  uart_put_char('x');
  uart_put_char(((b & 0xF0)>>4)+'0');
  uart_put_char('0'+(b & 0x0F));
*/
//  itoa(b,s,25);
s[0] = '0';
s[1] = 'x';
s[2] = (((b & 0xF0)>>4)+'0');
s[3] = ('0'+(b & 0x0F));
s[4] = 0;
  uart_put_string(s);
}

//////////////////////////////////////////////////////////////////////////7
void timer_init (void){
  OCR1A   =  (F_CPU / F_INTERRUPTS) - 1;                                    // compare value: 1/10000 of CPU frequency
  TCCR1B  = (1 << WGM12) | (1 << CS10);                                     // switch CTC Mode on, set prescaler to 1
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  TIMSK  = 1 << OCIE1A;                                                     // OCIE1A: Interrupt by timer compare
#else
  TIMSK1  = 1 << OCIE1A;                                                    // OCIE1A: Interrupt by timer compare
#endif  // __AVR...
}

//////////////////////////////////////////////////////////////////////////7
// Timer 1 output compare A interrupt service routine
ISR(TIMER1_COMPA_vect){
  (void) irmp_ISR();                                                        // call irmp ISR
}

//////////////////////////////////////////////////////////////////////////7
int main( void ){
//  TCCR0 = 1<<CS02;			//divide by 256
//  TIMSK = 1<<TOIE0;			//enable timer interrupt
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  UBRRL = bauddivider;			//set baud rate
  UBRRH = bauddivider >> 8;
  UCSRA = 0;				//no U2X, MPCM
  UCSRC = 1<<URSEL^1<<UCSZ1^1<<UCSZ0;	//8 Bit
  UCSRB = 1<<RXEN^1<<TXEN;		//enable RX, TX
#else
  UBRR0L = bauddivider;			//set baud rate
  UBRR0H = bauddivider >> 8;
  UCSR0A=0x00;
  UCSR0B=0x08;
  UCSR0C=0x06;
#endif  // __AVR...

  uart_put_string( "RC5-Dekoder:\n\r" );

  static char *Proto[]={"SIRCS","NEC","SAMSUNG","MATSUSH","KASEIKYO","RECS80","RC5(x)","DENON","RC6","SAMSG32","APPLE","RECS80X","NUBERT","B&O","GRUNDIG","NOKIA","SIEMENS","FDC","RCCAR","JVC","RC6A"};
  uart_put_string("IRMP V1.0\n\r");
  #if IRMP_LOGGING == 1
    uart_put_string("Logging Mode\n\r");
  #endif

  IRMP_DATA irmp_data;
  irmp_init();                                                              // initialize rc5
  timer_init();                                                             // initialize timer
  sei ();                                                                   // enable interrupts
  while(1){				// main loop
    if (irmp_get_data (&irmp_data)){
        // ir signal decoded, do something here...
        // irmp_data.protocol is the protocol, see irmp.h
        // irmp_data.address is the address/manufacturer code of ir sender
        // irmp_data.command is the command code
        #if IRMP_LOGGING != 1
            uart_put_string("\n\rCode: ");
            uart_put_string(Proto[irmp_data.protocol-1]);
            uart_put_string("\n\rAddress: ");
            uart_put_hex(irmp_data.address);
            uart_put_string("\n\rCommand: ");
            uart_put_hex(irmp_data.command);
//            uart_put_string("\n\rFlag: ");
//            uart_put_hex(irmp_data.flags);
            uart_put_string("\n\r");
        #endif
    }//end if
  }//end while
}//end main

