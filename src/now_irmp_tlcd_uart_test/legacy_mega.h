#ifndef _LEGACY_MEGA_H_
#define _LEGACY_MEGA_H_

    #if defined (__AVR_ATmega8__) || defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega64__) || defined (__AVR_ATmega162__) 
        #define UDR0            UDR
        #define UDRE0           UDRE           
        #define UBRR0L          UBRRL          
        #define UBRR0H          UBRRH          
        #define UCSR0A          UCSRA          
        #define UCSR0B          UCSRB          
        #define UCSR0C          UCSRC          
        #define UCSZ01          UCSZ1          
        #define UCSZ00          UCSZ0          
        #define RXEN0           RXEN           
        #define TXEN0           TXEN           
        #define RXCIE0          RXCIE          
        #define USART0_RX_vect  USART_RX_vect 
    //    #define USART0_TX_vect  USART_TX_vect 
    //    #define ??? -> URSEL          
    #endif

#endif

