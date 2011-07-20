/**********************************************************************
 * UART-Modul
 * default:   19200 Baud 8N1   
 * Processor: ATMEGA 8
 * CLOCK:     16 MHz
 * Version:   0.5 
 *********************************************************************/
#include "uart.h"

void uart_init(void){
  UBRRL = bauddivider;			            // set baud rate
  UBRRH = bauddivider >> 8;                 // set baud rate
  UCSRA = 0;				                // no U2X, MPCM
  UCSRC = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;	// No Parity | 1 Stop Bit | 8 Data Bit
  UCSRB = 1<<RXEN  | 1<<TXEN |	1<<RXCIE;// | 1<<TXCIE;//enable RX, TX and interr.
}
//*********************************************************************//
//void uart_init(void){
#if 0 //defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  UBRRL = bauddivider;			            // set baud rate
  UBRRH = bauddivider >> 8;                 // set baud rate
  UCSRA = 0;				                // no U2X, MPCM
  UCSRC = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;	// No Parity | 1 Stop Bit | 8 Data Bit
  UCSRB = 1<<RXEN  | 1<<TXEN;  
  // |	1<<RXCIE;// | 1<<TXCIE;  //enable RX, TX and interr.
//#else
  UBRR0L = bauddivider;			//set baud rate
  UBRR0H = bauddivider >> 8;
  UCSR0A=0x00;
  UCSR0B=0x08;
  UCSR0C=0x06;
#endif  // __AVR...
//}
//*********************************************************************//
char uart_check_char(void){
  //IF(Byte Empfangen)THEN RETURN Byte ELSE RETURN 0x00
  return (UCSRA & 0x80) ? UDR : 0x00;
}
//*********************************************************************//
char uart_get_char(void){
   while(!(UCSRA & 0x80)); // wait for received byte
   return UDR;
}
//*********************************************************************//
void uart_put_char(char c){
  while( (UCSRA & 1<<UDRE) == 0 ){;}
  UDR = c;
}
//*********************************************************************//
void uart_put_string(char *s){
  while(*s) uart_put_char(*s++);
}

//////////////////////////////////////////////////////////////////////////7
void uart_put_hex( char b ){
    //  itoa(b,s,25);
    s[0] = '0';
    s[1] = 'x';
    s[2] = (((b & 0xF0)>>4)+'0');
    s[3] = ('0'+(b & 0x0F));
    s[4] = 0;
    uart_put_string(s);
}

//********************************************************************//
/*
void uart_get_string(char* erg, const unsigned char max){
    char count = 0;
    char c = uart_get_char();
    while(c != 0x0D){ //Bis c = "Enter"
        if(c == 0x08 || c == 0x7F){ // Backspace
            if(count > 0){ //wenn min. ein Zeichen im Buffer
                erg--;
                *erg = 0x00;
                count--;
                uart_put_string("\b \b"); // Löschen im Terminal
            }else{
                uart_put_char(0x07);//beep, buffer schon leer
            }//end if
        }else if(c == 0x1B){ // ESC
            _delay_ms(3);
            c = uart_get_char();
            c = uart_get_char();
        }else if(count < max && c > 0x20 && c < 0x7F){
            *erg = c;
            erg++;
            count++;
            uart_put_char(c);//echo
        }else{
            uart_put_char(0x07);//beep, buffer schon voll
        }//end if
        c = uart_get_char();
    }//end while
    *erg = 0x00; //String Terminator!
}//end uart_getString
*/

