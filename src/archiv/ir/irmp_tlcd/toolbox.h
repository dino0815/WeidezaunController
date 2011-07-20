#ifndef TOOLBOX_H
#define TOOLBOX_H

//#include <stdlib.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define byte unsigned char
//typedef unsigned char byte;

#if 0 
    //Port D:
    #define RXD     0x01
    #define TXD     0x02
    #define Taster1 0x04
    #define Taster2 0x08
    #define Taster3 0x10
    #define LED1    0x20
    #define LED2    0x40
    #define SUMMER  0x80
#endif
    #define SUMMER  0x04
    #define IR      0x08
 
void init_ports(void);
void delay(long int count);
int sgn(int x);
void alarm(void);

#endif
