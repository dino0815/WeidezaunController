/**********************************************************************
 * Test Uart Modul (Echo Funktion)
 * default:   19200 Baud 8N1   
 * Processor: ATMEGA 8
 * CLOCK:     16 MHz
 * Version:   0.3 
 *********************************************************************/
//#include <stdlib.h>
//#include <stdio.h>
//#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h> // delay funktionen
//#include <avr/pgmspace.h> // In internen EEPROM schreiben
//#include <avr/wdt.h>  //Watchdoc
#include "uart.h"

//#define BAUD	19200

/*****************************************************************************/
ISR(USART_RXC_vect){
    char c = UDR;       // grap Char
    uart_put_char(c);   // echo
}

//*********************************************************************//
void init_ports(void){
    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
}

/***********************************************************************
 *  main programm
 **********************************************************************/
void main(void){
    init_ports();
    uart_init();
    uart_put_string("\r\n\r\nDinos Uart Modul v0.3:\r\n");
    uart_put_string("(Echo ON)\r\n");
    sei();
    while(1){
    }//while(1)
}//end main() 

