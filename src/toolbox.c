#include "toolbox.h"
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

//////////////////////////////////////////////////////////////////////////////
void delay(long int count){
  while(count > 0){count--;}
}//end delay() 

//////////////////////////////////////////////////////////////////////////////
int sgn(int x){
  return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}//end sgn(int)

//////////////////////////////////////////////////////////////////////////////
void alarm(void){
   short int i = 0;
   while(i<150){
      PORTD ^= SUMMER; 
      _delay_us(500); 
      i++;
  }//while(1)
}//end alarm()
