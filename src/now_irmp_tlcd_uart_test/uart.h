#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <inttypes.h>
//#include "legacy_mega.h"

//#define uchar unsigned char
//#define uint unsigned int
//#define byte unsigned char
//#define uint8 unsigned char
#define uint16 unsigned int
//#define uint16 uint16_t

#ifndef F_CPU
    #error F_CPU is not defined
    #warning F_CPU is not set ... Now set to 8000000
    #define F_CPU  8000000 // timer clock 8Mhz
#endif

#ifndef BAUD
    #error BAUD is not defined
    #warning BAUD is not defined ... Now set to 19200
    #define BAUD	19200
#endif

#define bauddivider (uint16_t)(F_CPU / BAUD / 16 - 0.5)

void uart_init(void);
char uart_check_byte(void); // Prüft und gibt sonst Null zurück
char uart_get_char(void);   // Wartet bis Byte empfangen wurde
void uart_put_char(char c); // Sendet ein Byte
void uart_put_string(char *s); // Sendet Null-Terminierten String
void uart_put_hex(  char b );

//void uart_get_string(char* erg, unsigned char max); //Blockiert bis String mit "Enter" abgeschlossen wird
//#define UART_RBUFFER_SIZE 8
//volatile char uart_rBuffer[UART_RBUFFER_SIZE];
//volatile unsigned char uart_rBuffer_rpos; //lese Position
//volatile unsigned char uart_rBuffer_wpos; //schreibe Position

#endif
