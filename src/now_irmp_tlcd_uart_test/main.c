/************************************************************************
   Text-LC-Display Controller 
   Processor: ATmega8
   CLOCK: 8MHZ
***************************************************************************/
#include "toolbox.h"
#include "tlcd.h"

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

/*
 *   int i =0;
  while(1){
    tlcd_putChar((char)i);
    //PORTD ^= LED1 | LED2;// | SUMMER; 
    _delay_ms(500); 
    i++;
  }//while(1)

 * */
