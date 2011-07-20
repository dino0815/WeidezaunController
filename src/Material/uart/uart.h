#ifndef _UART_H_
#define _UART_H_

//#include <inttypes.h>
#include <avr/io.h>

#define uchar unsigned char
#define uint unsigned int
typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned int  uint16;

#ifndef F_CPU
    #error F_CPU is not defined
//  #define F_CPU  16000000 // timer clock 16Mhz
#endif

#ifndef BAUD
    #error BAUD is not defined
//  #define BAUD	19200
#endif

#define bauddivider (uint16)(F_CPU / BAUD / 16 - 0.5)

void uart_init(void);
char uart_check_byte(void); // Prüft und gibt sonst Null zurück
char uart_get_char(void);   // Wartet bis Byte empfangen wurde
void uart_put_char(char c); // Sendet ein Byte
//void uart_get_string(char* erg, unsigned char max); //Blockiert bis String mit "Enter" abgeschlossen wird
void uart_put_string(char *s); // Sendet Null-Terminierten String

//#define UART_RBUFFER_SIZE 8
//volatile char uart_rBuffer[UART_RBUFFER_SIZE];
//volatile unsigned char uart_rBuffer_rpos; //lese Position
//volatile unsigned char uart_rBuffer_wpos; //schreibe Position

#endif
