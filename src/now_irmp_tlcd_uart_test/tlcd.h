//HD44780 

#ifndef TLCD_H
#define TLCD_H
#include <avr/io.h>
#include "toolbox.h"

//TODO: Create Flag to define the data position in Hi- or Lo-Nibble in 4-Bit mode 
// now only Low-Nibble can use
#define TLCD_DBUS PORTD
#define TLCD_SBUS PORTB

#define RS (1<<PB6)//0x04
#define RW 0x00
#define E1 (1<<PB7)//0x01
#define E2 0x00

void tlcd_init4bit(void); //DBUS:4bit 
void tlcd_init8bit(void); //DBUS:8bit
void tlcd_put_char(char c);  
void tlcd_put_hex(char b);
void tlcd_put_string(char* s);  
void tlcd_put_command(byte b); 

void tlcd_ON(void); 
void tlcd_OFF(void); 

void tlcd_Cursor_ON(void); 
void tlcd_Cursor_OFF(void); 
void tlcd_Cursor_BLINK(void); 

void tlcd_DisplayShift_ON(void); 
void tlcd_DisplayShift_OFF(void); 

void tlcd_Direction_Left(void); 
void tlcd_Direction_Right(void); 

void tlcd_clear(void); 
void tlcd_set_position(short int zeile,short int pos); 

#endif

