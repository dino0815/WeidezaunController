//HD44780 

#ifndef TLCD_H
#define TLCD_H
#include <avr/io.h>
#include "toolbox.h"

#define TLCD_DBUS PORTA
#define TLCD_SBUS PORTB

#define RS 0x04
#define RW 0x02
#define E1 0x01
#define E2 0x00

void tlcd_init4bit(void); //DBUS:4bit 
void tlcd_init8bit(void); //DBUS:8bit
void tlcd_putChar(char c);  
void tlcd_putString(char* s);  
void tlcd_putCommand(byte b); 

void tlcd_ON(void); 
void tlcd_OFF(void); 

void tlcd_Cursor_ON(void); 
void tlcd_Cursor_OFF(void); 
void tlcd_Cursor_BLINK(void); 

void tlcd_DisplayShift_ON(void); 
void tlcd_DisplayShift_OFF(void); 

void tlcd_Direction_Left(void); 
void tlcd_Direction_Right(void); 

void tlcd_Clear(void); 
void tlcd_setPosition(short int zeile,short int pos); 

#endif

