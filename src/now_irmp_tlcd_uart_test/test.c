/************************************************************************
 * Test IRMP mit Uart und TLCD Modul
 * Processor: ATMEGA 168
 * CLOCK:     8 MHz
 * Version:   0.2 
 ************************************************************************/ 
//#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h> // delay funktionen
#include <avr/pgmspace.h> // In internen EEPROM schreiben
#include <avr/interrupt.h>
//#include <avr/wdt.h>  //Watchdoc
#include "uart.h"
#include "tlcd.h"
#include "irmp.h"
#include "irmpconfig.h"

#define uchar unsigned char
#define uint unsigned int
#define byte unsigned char
#define uint8 unsigned char
#define uint16 unsigned int
//typedef unsigned char byte;
//typedef unsigned char uint8;
//typedef unsigned int  uint16;

#define IR_NEC                        2              // NEC, Pioneer, JVC, Toshiba, NoName etc.
#define IR_RC5                        7              // Philips etc
//char s[30];
static char *Proto[]={"SIRCS","NEC","SAMSUNG","MATSUSH","KASEIKYO","RECS80","RC5","DENON","RC6","SAMSG32","APPLE","RECS80X","NUBERT","B&O","GRUNDIG","NOKIA","SIEMENS","FDC","RCCAR","JVC","RC6A"};
IRMP_DATA irmp_data;

//////////////////////////////////////////////////////////////////////////////
void init_ports(void){
    /* initialize ports */
//    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
//    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
}//end init_ports()

///////////////////////////////////////////////////////////////////////////
void timer_init (void){
  OCR1A   =  (F_CPU / F_INTERRUPTS) - 1;                                    // compare value: 1/10000 of CPU frequency
  TCCR1B  = (1 << WGM12) | (1 << CS10);                                     // switch CTC Mode on, set prescaler to 1
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  TIMSK  = 1 << OCIE1A;                                                     // OCIE1A: Interrupt by timer compare
#else
  TIMSK1  = 1 << OCIE1A;                                                    // OCIE1A: Interrupt by timer compare
#endif  // __AVR...

//  TCCR0 = 1<<CS02;			//divide by 256
//  TIMSK = 1<<TOIE0;			//enable timer interrupt
}

///////////////////////////////////////////////////////////////////////////
// Timer 1 output compare A interrupt service routine
ISR(TIMER1_COMPA_vect){
  (void) irmp_ISR();                                                        // call irmp ISR
}

///////////////////////////////////////////////////////////////////////////
char ir_data(char codec, int add, int com, char flag){
    if(irmp_data.protocol == codec && irmp_data.address == add && irmp_data.command == com && irmp_data.flags == flag) return 1;
    else return 0;
}
///////////////////////////////////////////////////////////////////////////
void main( void ){
  init_ports();    
  uart_init();
  tlcd_init4bit();  
  irmp_init();                                                              // initialize rc5
  timer_init();                                                             // initialize timer

  // Das 1x10 Display läuft als zweizeiliges Display mit je 5 Zeichen.
  tlcd_set_position(1,0);
  tlcd_put_string("Berei"); 
  tlcd_set_position(2,0);
  tlcd_put_string("t!"); 

  uart_put_string( "WeidezaunController, Bereit!\n\r" );
  #if IRMP_LOGGING == 1
    uart_put_string("Logging Mode\n\r");
  #endif

  sei ();                                                                   // enable interrupts
  while(1){				// main loop
    if (irmp_get_data (&irmp_data) && irmp_data.flags == 0){
      // ir signal decoded, do something here...
      // irmp_data.protocol is the protocol, see irmp.h
      // irmp_data.address is the address/manufacturer code of ir sender
      // irmp_data.command is the command code
      // irmp_data.flags is 0 at new command and 1 at repeat command
      #if IRMP_LOGGING != 1
        if(ir_data(IR_RC5,0x08,0x20,0)){
          tlcd_set_position(1,0);
          tlcd_put_string("Zaun "); 
          tlcd_set_position(2,0);
          tlcd_put_string(" ON  "); 
          uart_put_string("Commando; RC5, 8, 0x20 erkannt! ;)");

        }else if(ir_data(IR_RC5,0x08,0x21,0)){
          tlcd_set_position(1,0);
          tlcd_put_string("Zaun "); 
          tlcd_set_position(2,0);
          tlcd_put_string(" OFF "); 
          uart_put_string("Commando; RC5, 8, 0x21 erkannt! ;)");

        }else if(ir_data(IR_RC5,0x00,0x11,0)){
          tlcd_set_position(1,0);
          tlcd_put_string("Batt."); 
          tlcd_set_position(2,0);
          tlcd_put_string("-----"); 
          uart_put_string("Commando; RC5, 0, 0x11 erkannt! ;)");

        }else{
          // Gibt erkanntes IR-Paket auf TextDisplay aus: 
          tlcd_clear(); 
          tlcd_set_position(1,0);
          tlcd_put_string(Proto[irmp_data.protocol-1]); 
          tlcd_set_position(2,0);
          tlcd_put_hex(irmp_data.address);
          tlcd_put_char(';');
          tlcd_put_hex(irmp_data.command);          
        }//end if 
      #endif
    }//end if(irmp_get_data...
  }//end while
}//end main

