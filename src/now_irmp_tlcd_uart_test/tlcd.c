#include "tlcd.h"

short int zeile;
short int position;
char status;
//xxxxxxx1 Zeile
//xxxxxx1x Zeile (nur bei 4 Zeilen Displays)
//xxxxx1xx Shift Left
//xxxx1xxx Display Shift 
//xxx1xxxx Display ON
//xx1xxxxx Cursor Blinking
//x1xxxxxx Cursor ON
//1xxxxxxx 4bit Modus
#define TLCD_IS_4BIT   status & 0x80
#define TLCD_IS_DSHIFT status & 0x08
#define TLCD_IS_RSHIFT status & 0x04
#define TLCD_IS_ON     status & 0x10
#define TLCD_IS_BLINK  status & 0x20
#define TLCD_IS_CURSOR status & 0x40

#define TLCD_SET_4BIT      status |= 0x80
#define TLCD_SET_8BIT      status &= 0x7F
#define TLCD_SET_DSHIFT    status |= 0x08
#define TLCD_SET_NO_DSHIFT status &= 0xF7
#define TLCD_SET_RSHIFT    status |= 0x04
#define TLCD_SET_LSHIFT    status &= 0xFB
#define TLCD_SET_ON        status |= 0x10
#define TLCD_SET_OFF       status &= 0xEF
#define TLCD_SET_BLINK     status |= 0x20
#define TLCD_SET_NO_BLINK  status &= 0xDF
#define TLCD_SET_CURSOR    status |= 0x40
#define TLCD_SET_NO_CURSOR status &= 0xBF

////////////////////////////////////////////////////////////////////////
void tlcd_ON(void){
   char b = 0x00;
   TLCD_SET_ON;
   b |= 0x08; // Befehl 4
   b |= 0x04; // Display ON
   if(TLCD_IS_CURSOR) b |= 0x02; // Cursor ON
   if(TLCD_IS_BLINK)  b |= 0x01; // Cursor Blink
   tlcd_put_command(b);    
}//end tlcd_ON()

////////////////////////////////////////////////////////////////////////
void tlcd_OFF(void){
   char b = 0x00;
   TLCD_SET_OFF;
   b |= 0x08; // Befehl 4
   tlcd_put_command(b);    
}//end tlcd_OFF()

////////////////////////////////////////////////////////////////////////
void tlcd_Cursor_ON(void){
   char b = 0x00;
   TLCD_SET_CURSOR;
   b |= 0x08;                    // Befehl 4
   if(TLCD_IS_ON) b |= 0x04;     // Display ON
   b |= 0x02;                    // Cursor ON
   if(TLCD_IS_BLINK) b |= 0x01;  // Cursor Blink
   tlcd_put_command(b);    
}//end tlcd_Cursor_ON()

////////////////////////////////////////////////////////////////////////
void tlcd_Cursor_OFF(void){
   char b = 0x00;
   TLCD_SET_NO_CURSOR;
   b |= 0x08;                    // Befehl 4
   if(TLCD_IS_ON) b |= 0x04;     // Display ON
   tlcd_put_command(b);    
}//end tlcd_Cursor_OFF()

////////////////////////////////////////////////////////////////////////
void tlcd_Cursor_BLINK(void){
   char b = 0x00;
   TLCD_SET_BLINK;
   b |= 0x08;                    // Befehl 4
   if(TLCD_IS_ON) b |= 0x04;     // Display ON
   b |= 0x02;                    // Cursor ON
   b |= 0x01;                    // Cursor Blink
   tlcd_put_command(b);    
}//end tlcd_Cursor_BLINK()

////////////////////////////////////////////////////////////////////////
void tlcd_Cursor_NO_BLINK(void){
   char b = 0x00;
   TLCD_SET_NO_BLINK;
   b |= 0x08;                    // Befehl 4
   if(TLCD_IS_ON) b |= 0x04;     // Display ON
   if(TLCD_IS_CURSOR) b |= 0x02; // Cursor ON
   tlcd_put_command(b);    
}//end tlcd_Cursor_BLINK()

////////////////////////////////////////////////////////////////////////
void tlcd_DisplayShift_ON(void){
   char b = 0x00;
   TLCD_SET_DSHIFT;  
   b |= 0x04;                    // Befehl 3
   if(TLCD_IS_RSHIFT) b |= 0x02; // Direction_Right
   b |= 0x01;                    // Display SHIFT
   tlcd_put_command(b);    
}//end tlcd_DisplayShift_ON()

////////////////////////////////////////////////////////////////////////
void tlcd_DisplayShift_OFF(void){
   char b = 0x00;
   TLCD_SET_NO_DSHIFT;  
   b |= 0x04;                    // Befehl 3
   if(TLCD_IS_RSHIFT) b |= 0x02; // Direction_Right
   tlcd_put_command(b);    
}//end tlcd_DisplayShift_OFF()

////////////////////////////////////////////////////////////////////////
void tlcd_Direction_Left(void){
   char b = 0x00;
   TLCD_SET_LSHIFT;  
   b |= 0x04;                    // Befehl 3
   if(TLCD_IS_DSHIFT) b |= 0x01; // Display SHIFT
   tlcd_put_command(b);    
}//end tlcd_Direction_Left()

////////////////////////////////////////////////////////////////////////
void tlcd_Direction_Right(void){
   char b = 0x00;
   TLCD_SET_RSHIFT;  
   b |= 0x04;                    // Befehl 3
   b |= 0x02;                    // Direction_Right
   if(TLCD_IS_DSHIFT) b |= 0x01; // Display SHIFT
   tlcd_put_command(b);    
}//end tlcd_Direction_Right()

////////////////////////////////////////////////////////////////////////
void tlcd_clear(void){
   tlcd_put_command(0x01); // Display/Speicher Löschen
   _delay_ms(2); 
}//end tlcd_clear()

////////////////////////////////////////////////////////////////////////
void tlcd_set_position(short int zeile,short int pos){
   char b = 0x00;
   b |= 0x80; // Befehl 8
   if(pos>40)pos=0;
   if(zeile==1){
      b |= pos;
   }else if(zeile==2){
      pos=pos+40;
      b |= pos;
   }else{
    //  alarm();
   }
   tlcd_put_command(b);    
}//end tlcd_set_position(short int)

////////////////////////////////////////////////////////////////////////
void tlcd_clock(void){
   TLCD_SBUS |= E1;
   _delay_us(5); 
   TLCD_SBUS &= ~E1;
   _delay_us(50); 
}//end clock

////////////////////////////////////////////////////////////////////////
void tlcd_put_command(char b){
   TLCD_SBUS = 0x00;
   if(TLCD_IS_4BIT){
      TLCD_DBUS = b & 0xF0;
      tlcd_clock();
      TLCD_DBUS = (b << 4)& 0xF0;
   }else{
      TLCD_DBUS = b;
   }//end if
   tlcd_clock();
}//end tlcd_put_command()

////////////////////////////////////////////////////////////////////////
void tlcd_put_char(char c){   
   TLCD_SBUS = RS; 
   if(TLCD_IS_4BIT){
      TLCD_DBUS = c & 0xF0;
      tlcd_clock();
      TLCD_DBUS = (c << 4)& 0xF0;
   }else{
      TLCD_DBUS = c;
   }//end if
   tlcd_clock();
}//put_char()

////////////////////////////////////////////////////////////////////////
void tlcd_put_string(char* s){  
  while(*s != '\0'){
    tlcd_put_char(*s);
    s++;
  }//end for
}//put_char()

//////////////////////////////////////////////////////////////////////////7
void tlcd_put_hex( char b ){
    // obere 4 bit:
    char temp = ((b >> 4)& 0x0F);
    if(temp < 10) tlcd_put_char(temp +'0');
    else     tlcd_put_char(temp + '0' + 7);
    // untere 4 bit:
    temp = (b & 0x0F);
    if(temp < 10) tlcd_put_char(temp +'0');
    else     tlcd_put_char(temp + '0' + 7);
}

////////////////////////////////////////////////////////////////////////
void tlcd_init8(void){   
   status=0x00;
   position=0;
   zeile=1;
   TLCD_SET_8BIT;
   _delay_ms(15); // Warten bis Display-Reset sicher vorbei ist!
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   _delay_ms(5); 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   _delay_ms(1); 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 

   tlcd_OFF();
   tlcd_clear();
   tlcd_put_command(0x02); // Cursor and Display on Position 0
   _delay_ms(2); 
   tlcd_ON();
}//tlcd_init8()
 
////////////////////////////////////////////////////////////////////////
void tlcd_init4bit(void){
   status=0x00;
   position=0;
   zeile=1;
   TLCD_SET_8BIT;
   _delay_ms(15); // Warten bis Display-Reset sicher vorbei ist!
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   _delay_ms(5); 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   _delay_ms(1); 
   tlcd_put_command(0x38); // 8bit,2Zeilen,5x8 
   tlcd_put_command(0x28); // 4bit,2Zeilen,5x8 

   TLCD_SET_4BIT;
   tlcd_put_command(0x28); // 4bit,2Zeilen,5x8 
   
   tlcd_OFF();
   tlcd_clear();
   tlcd_put_command(0x02); // Cursor and Display on Position 0
   _delay_ms(2); 
   tlcd_ON();
}//tlcd_init4()
 
