/*****************************************************
	SmileyFlashWrite.h 6/3/10 Joe Pardue
******************************************************/

// I HATE LICENSES LIKE THIS >>BUT<< I've been told that without
// the license then the work is automatically copyrighted in my name.
// But since my purpose is to educate (and learn), I want the code to be 
// used by whoever wants to use it to learn something. If you like it, 
// then visit my website www.smileymicros.com and buy something.

/*
 *  BSD License
 *  -----------
 *
 *  Copyright (c) 2010, Smiley Micros, All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 *   
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *   
 *  - Neither the name of the Smiley Micros nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// And to further cover my butt, let me add that if you use this software
// it will destroy whatever machine you use it on and kill anyone in a one 
// kilometer radius. So don't even consider using it for any reason whatsoever!

       
#include <avr/pgmspace.h> 
#include <avr/wdt.h>
#include <avr/boot.h>

/*****************************************************
	To use this source code you must select the device
	in the #define list AND you must set the processor
	for the device in the Configuration Options
	
	Select the device from the #define list:
	Remove '//' from device you are using
	Add '//' to devices you are not using

	Select the processor from Projects menu item 
	Configuration Options - Device:
	Butterfly - ATmega169
	Arduino - ATmega328
	BeAVR - ATmega644

	You must make both selections for this to compile
	properly in AVRStudio
******************************************************/
//#define Butterfly // ATmega169
#define Arduino // ATmega328
//#define BeAVR40 // ATmega644

/*****************************************************
	IMPORTANT REMINDER:
	In AVRStudio you have to set the Project Configuration
	Options to the memory location for the boot section.
	Butterfly 0x1C00
	Arduino 0x3C00
	BeAVR40 0x7C00
******************************************************/


// SmileyUSART.h uses the above defines
// Note that Butterfly USART runs at 19200 Baud
// while the Arduino and BeAVR40 run at 57600 Baud

// Strings stored in program memory to use as startup greeting
#if defined(Arduino)
const char HELLO[] PROGMEM = "SmileyFlashTest - Arduino 1.01\n";
#elif defined (Butterfly)
const char HELLO[] PROGMEM = "SmileyFlashTest - Butterfly 1.03\n";
#elif defined (BeAVR40)
const char HELLO[] PROGMEM = "SmileyFlashTest - BeAVR40 1.08\n";
#else
	#error "ERROR: You MUST define a device!";
#endif

// Constants used to calculate the register
// settings for each device baudrate
#if defined(Butterfly)
#include <avr/interrupt.h>
#define F_CPU 8000000
#define BAUD 19200
#elif (defined(Arduino) || defined(BeAVR40))
#define F_CPU 16000000L
#define BAUD 57600
#else
	#error "F_CPU and BAUD undefined"
#endif 


/*****************************************************
	Make register names more readable
	From Mike Perks, a generous AVRFreak
	 - modified quite a bit
******************************************************/
/* defines for the various USART registers */

#if defined(Butterfly)

   // ATMega with one USART 
   #define USART_BAUD_RATE_HIGH        	UBRRH
   #define USART_BAUD_RATE_LOW        	UBRRL
   #define USART_CONTROL_STATUS_REG_A	UCSRA
   #define USART_CONTROL_STATUS_REG_B	UCSRB
   #define USART_CONTROL_STATUS_REG_C	UCSRC
   #define USART_ENABLE_TRANSMITTER		TXEN
   #define USART_ENABLE_RECEIVER		RXEN
   #define USART_READY_TO_TRANSMIT		UDRE
   #define USART_TRANSMIT_COMPLETE		TXC
   #define USART_RECEIVE_COMPLETE		RXC
   #define USART_DATA_REG				UDR
   #define USART_STOP_BIT_SELECT 		USBS
   #define USART_CHARACTER_SIZE_0 		UCSZ0
   #define USART_CHARACTER_SIZE_1 		UCSZ1
   #define USART_REGISTER_SELECT		URSEL

#elif defined(BeAVR40) || defined(Arduino)

   // ATMega with two USARTs - USART 0
   #define USART_BAUD_RATE_HIGH			UBRR0H
   #define USART_BAUD_RATE_LOW			UBRR0L
   #define USART_CONTROL_STATUS_REG_A	UCSR0A
   #define USART_CONTROL_STATUS_REG_B	UCSR0B
   #define USART_CONTROL_STATUS_REG_C	UCSR0C
   #define USART_ENABLE_TRANSMITTER		TXEN0
   #define USART_ENABLE_RECEIVER		RXEN0
   #define USART_READY_TO_TRANSMIT		UDRE0
   #define USART_TRANSMIT_COMPLETE		TXC0
   #define USART_RECEIVE_COMPLETE		RXC0
   #define USART_DATA_REG				UDR0
   #define USART_STOP_BIT_SELECT		USBS0
   #define USART_CHARACTER_SIZE			UCSZ00

#else
   #error "no USART definition for MCU"
#endif


/*****************************************************
	Function declarations	 
******************************************************/
static void Initialization();
static void USARTInit();
static void sendByte( uint8_t );
static uint8_t receiveByte( void );
void sendStringP(const char *); 

#if defined(Butterfly)
void OSCCAL_calibration(void);
void Delay(unsigned int) ;
#endif

static void blockFlashLoad(uint16_t size);
static void blockFlashRead(uint16_t size);

uint8_t isByteAvailable();

/*****************************************************
	Global variable Declarations
******************************************************/

// SRAM Page buffer for flash page pre-load
uint8_t pageBuffer[SPM_PAGESIZE];

// AVR109 uses a global address
uint16_t address;

// use address well above location in application section
// and well below bootsection
#if defined (Arduino) || defined (BeAVR40)
	#define TEST_ADDRESS 0x2000
#elif defined(Butterfly)
	#define TEST_ADDRESS 0x300
#endif


