/************************************************************************
   Text-LC-Display Controller 
   Processor: ATmega8
   CLOCK: 8MHZ
***************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "tlcd.h"

#define uchar unsigned char
#define uint  unsigned int
#define byte  unsigned char
//typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned int  uint16;

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

/************************************************************************
   main programm
***************************************************************************/
int main(void){
   init_ports();    
   tlcd_init4bit();  

   _delay_ms(1000);  
   tlcd_setPosition(1,0);
   tlcd_putString("Hallo Welt!"); 
   _delay_ms(1000);  
   tlcd_setPosition(2,0);
   tlcd_putString("doch nicht :)"); 

  while(1){
  }//while(1)
}//end main() 
