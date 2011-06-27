/************************************************************************/
/*                                                                      */
/*                      RC5 Remote Receiver                             */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#include <io.h>
#include <interrupt.h>
#include <signal.h>
#include <stdlib.h>


#define uchar unsigned char
#define uint unsigned int

#define	xRC5_IN		PIND
#define	xRC5		PD7			// IR input low active

#define	XTAL		11.0592e6
//#define	XTAL		7.3728e6
//#define XTAL		5e6

#define	BAUD	19200
#define bauddivider (uint)(XTAL / BAUD / 16 - 0.5)


extern uint	rc5_data;				// store result
