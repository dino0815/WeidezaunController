/**********************************************************************
 * UART-Modul
 * default:   19200 Baud 8N1   
 * Processor: ATMEGA 8
 * CLOCK:     16 MHz
 * Version:   0.5 
 *********************************************************************/
#include "uart.h"

//*********************************************************************//
void uart_init(void){
#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
  UBRRL = bauddivider;			            // set baud rate
  UBRRH = bauddivider >> 8;                 // set baud rate
  UCSRA = 0;				                // no U2X, MPCM
  UCSRC = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;	// No Parity | 1 Stop Bit | 8 Data Bit
  UCSRB = 1<<RXEN  | 1<<TXEN;  // |	1<<RXCIE;// | 1<<TXCIE;  //enable RX, TX and interr.
#else // (ATmega48, ATmega88, ATmega168, ATmega328, ATmega644)
  UBRR0L = bauddivider;      // set baud rate
  UBRR0H = bauddivider >> 8; // set baud rate
  UCSR0A = 0;                // no U2X, MPCM
  UCSR0C = 1<<UCSZ01 | 1<<UCSZ00;	// No Parity | 1 Stop Bit | 8 Data Bit
  UCSR0B = 1<<RXEN0  | 1<<TXEN0; // |	1<<RXCIE0;// | 1<<TXCIE;//enable RX, TX and interr.
#endif  // __AVR...
}
//*********************************************************************//
char uart_check_char(void){
  return (UCSR0A & 0x80) ? UDR0 : 0x00;    //IF(Byte Empfangen)THEN RETURN Byte ELSE RETURN 0x00
}
//*********************************************************************//
char uart_get_char(void){
   while(!(UCSR0A & 0x80)); // wait for received byte
   return UDR0;
}
//*********************************************************************//
void uart_put_char(char c){
  while( (UCSR0A & 1<<UDRE0) == 0 ){;}
  UDR0 = c;
}
//*********************************************************************//
void uart_put_string(char *s){
  while(*s) uart_put_char(*s++);
}
//////////////////////////////////////////////////////////////////////////7
void uart_put_hex( char b ){
    // obere 4 bit:
    char temp = ((b >> 4) & 0x0F);
    if(temp < 10) uart_put_char(temp +'0');
    else     uart_put_char(temp + '0' + 7);
    // untere 4 bit:
    temp = (b & 0x0F);
    if(temp < 10) uart_put_char(temp +'0');
    else     uart_put_char(temp + '0' + 7);
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

