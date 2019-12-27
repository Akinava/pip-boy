/*****************************************************
	SmileyFlashTest.c Joe Pardue 6/3/10
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

#include "SmileyFlashTest.h"

int main(void) 
{ 
	uint8_t b;
	uint16_t i;

	// number of bytes to store and read
	uint8_t size = 16;

	Initialization();

	// Send the hello string selected by the #define
	sendStringP(HELLO);
	// Send a test string
	sendStringP(PSTR("This is a test for sending a string.\n"));

	while(1)
	{
		// receiveByte waits for a byte 
		if(isByteAvailable())
		{
			b = receiveByte();
			if( b == 'r') // Read 16 bytes
			{
				address = TEST_ADDRESS;
				blockFlashRead(size);
			}
			else if(b == 'w') // Write increasing sequence
			{
				// create and store sequence 0 to 0x0F
				for (i=0; i<size; i++) 
				{
					pageBuffer[i]=i;
				}

				address = TEST_ADDRESS;

				blockFlashLoad(size);
			}	
			else if(b == 'W') // Write zeros
			{
				// create and store zeros
				for (i=0; i<size; i++) 
				{
					pageBuffer[i]=0;
				}
				address = TEST_ADDRESS;

				blockFlashLoad(size);
			}			
			else sendByte('?');
		}			
	}
}

/*****************************************************
	Flash Functions
******************************************************/
void blockFlashLoad(uint16_t size)
{ 
	// address is global and set elsewhere
	uint16_t tempAddress = address;	
	uint16_t i,tempWord;

	// Perform page erase
    boot_page_erase(address);

	// Wait until the memory is erased
    boot_spm_busy_wait();		

	// fill the flash page buffer with the data
    for(i = 0; i < size; i+=2)
	{
		// load the little end byte from the word
		tempWord = pageBuffer[i];

		// load the big end byte
		tempWord += (pageBuffer[i+1] << 8);
		
		// put the word into the page butter
        boot_page_fill(address,tempWord);;
    	
		// incrememnt the word address						
        address = address + 2;
    } 
    
	// write the page to flash
    boot_page_write(tempAddress);

	// wait until finished writing
    boot_spm_busy_wait();
	
	// Re-enable the RWW section 
    boot_rww_enable();
					  
	// DEBUG send ! to shown you are done
	//sendByte('!');
}

void blockFlashRead(uint16_t size)
{ 
	uint8_t data;

	do {
		// read program memory byte at address
		data = pgm_read_byte_near(address++);

		// send the byte out the serial port
		sendByte(data);
			
		// reduce number of bytes to read by one		   				
		size--;	

		// loop while size is greater than 0						
	} while (size);						
}



/*****************************************************
	USART Functions
******************************************************/
void USARTInit()
{
	// Set Baud registers
	USART_BAUD_RATE_HIGH = (F_CPU/(BAUD*16L)-1) >> 8;
	USART_BAUD_RATE_LOW = (uint8_t)(F_CPU/(BAUD*16L)-1);

	// Enable receiver and transmitter
	USART_CONTROL_STATUS_REG_B = (1 << USART_ENABLE_RECEIVER) | (1 << USART_ENABLE_TRANSMITTER);
	// Set frame format: n,8,1
#if defined(BeAVR40) || defined(Arduino)
	USART_CONTROL_STATUS_REG_C |= (1 << USART_STOP_BIT_SELECT) | (1 << USART_CHARACTER_SIZE);  
#elif defined(ATmega32)
	USART_CONTROL_STATUS_REG_C |= (1 << USART_REGISTER_SELECT) | (1 << USART_CHARACTER_SIZE_0) | (1 << USART_CHARACTER_SIZE_1);

#elif defined(Butterfly)
	// for the Butterfly reuse the Atmel code for the USART in the intialization process
    // Increase the oscillator to 2 Mhz for the 19200 baudrate:  
    CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
    // set prescaler = 4, Inter RC 8Mhz / 4 = 2Mhz
    CLKPR = (1<<CLKPS1);    

     // Enable 2x speed change
    UCSRA = (1<<U2X);

    // Enable receiver and transmitter
    UCSRB = (1<<RXEN)|(1<<TXEN)|(0<<RXCIE)|(0<<UDRIE);

    // Set the USART to asynchronous at 8 bits no parity and 1 stop bit
    UCSRC = (0<<UMSEL)|(0<<UPM0)|(0<<USBS)|(3<<UCSZ0)|(0<<UCPOL);
#else
   #error "No USART definition for MCU in USARTInit"
#endif 
}

uint8_t isByteAvailable()
{
	// Does the RX0 bit of the USART Status and Control Register
	// indicate a char has been received?
	if ( (USART_CONTROL_STATUS_REG_A & (0x80)) ) return 1;
	return 0;
}

uint8_t receiveByte( void )
{
	return USART_DATA_REG;
}

void sendByte( uint8_t data )
{
	wdt_reset(); // reset the watchdog timer, if it is set
	// Wait for empty transmit buffer 
	while ( !( USART_CONTROL_STATUS_REG_A & (1<<USART_READY_TO_TRANSMIT)) );
	// Put data into buffer, sends the data
	USART_DATA_REG = data;
}

void sendStringP(const char *FlashString)
{
	int i = 0;

	// The 'for' logic terminates if the byte is '\0' or if i = 80.
	// '\0' is 'null' and terminates C strings
	// The 80 prevents too much overrun if we get a bad pointer
	// and it limits the string size	
	for( i = 0 ; pgm_read_byte(&FlashString[i]) && i < 80; i++) 
	{
		sendByte(pgm_read_byte(&FlashString[i]));
	}
}

/*****************************************************
	Initialization
******************************************************/

void Initialization()
{
	USARTInit();

#if defined(Butterfly)
// NOTE this all copied directly from the original Butterfly source code

    OSCCAL_calibration();   // Calibrate the OSCCAL byte

    ACSR = (1<<ACD);

    // Disable Digital input on PF0-2 (power save)
    DIDR1 = (7<<ADC0D);

    PORTB = 0xFF;       // Enable pullups on Ports B and E
    PORTE = 0xFF;
    
    // boost IntRC to 2Mhz to achieve 19200 baudrate
    CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
    // set prescaler = 4, Inter RC 8Mhz / 4 = 2Mhz
    CLKPR = (1<<CLKPS1);    

    UBRRH = 0;//(unsigned char)(baudrate>>8);
    UBRRL = 12;//(unsigned char)baudrate;

    // Enable 2x speed
    UCSRA = (1<<U2X);

    // Enable receiver and transmitter
    UCSRB = (1<<RXEN)|(1<<TXEN)|(0<<RXCIE)|(0<<UDRIE);

    // Async. mode, 8N1
    UCSRC = (0<<UMSEL)|(0<<UPM1)|(0<<USBS)|(3<<UCSZ0)|(0<<UCPOL);
        
    MCUCR = (1<<IVCE);       
    MCUCR = (1<<IVSEL);             //move interruptvectors to the Boot sector    

    sei(); // mt __enable_interrupt();
    
    // mt PCMSK1 = (1<<PORTB6) | (1<<PORTB4);
	PCMSK1 = (1<<PINB6) | (1<<PINB4);       // set pin-change interrupt mask
  	EIFR   = (1<<PCIF1);                    // clear external intterupt flag 1
	EIMSK  = (1<<PCIE1);   

#endif
}


// Only compile if Butterfly is defined
#if defined(Butterfly)
/*****************************************************
	Oscillator Calibration
	The following code is taken directly from Martin Thoma's
	port of Atmel's IAR Butterfly software to GCC
******************************************************/
/*****************************************************************************
*
*   Function name : OSCCAL_calibration
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Calibrate the internal OSCCAL byte, using the external 
*                   32,768 kHz crystal as reference
*
*****************************************************************************/
void OSCCAL_calibration(void)
{
    unsigned char calibrate = 0;//FALSE;
    int temp;
    unsigned char tempL;

    CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
    // set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
    CLKPR = (1<<CLKPS1) | (1<<CLKPS0);
    
    TIMSK2 = 0;             //disable OCIE2A and TOIE2

    ASSR = (1<<AS2);        //select asynchronous operation of timer2 (32,768kHz)
    
    OCR2A = 200;            // set timer2 compare value 

    TIMSK0 = 0;             // delete any interrupt sources
        
    TCCR1B = (1<<CS10);     // start timer1 with no prescaling
    TCCR2A = (1<<CS20);     // start timer2 with no prescaling

    while((ASSR & 0x01) | (ASSR & 0x04));       //wait for TCN2UB and TCR2UB to be cleared

    Delay(1000);    // wait for external crystal to stabilise
    
    while(!calibrate)
    {
        cli(); // mt __disable_interrupt();  // disable global interrupt
        
        TIFR1 = 0xFF;   // delete TIFR1 flags
        TIFR2 = 0xFF;   // delete TIFR2 flags
        
        TCNT1H = 0;     // clear timer1 counter
        TCNT1L = 0;
        TCNT2 = 0;      // clear timer2 counter
           
	while ( !(TIFR2 & (1<<OCF2A)) ); // while ( !(TIFR2 && (1<<OCF2A)) );   // wait for timer2 compareflag
    
        TCCR1B = 0; // stop timer1

        sei(); // __enable_interrupt();  // enable global interrupt
    
        if ( (TIFR1 & (1<<TOV1)) ) // if ( (TIFR1 && (1<<TOV1)) )
        {
            temp = 0xFFFF;      // if timer1 overflows, set the temp to 0xFFFF
        }
        else
        {   // read out the timer1 counter value
            tempL = TCNT1L;
            temp = TCNT1H;
            temp = (temp << 8);
            temp += tempL;
        }
    
        if (temp > 6250)
        {
            OSCCAL--;   // the internRC oscillator runs to fast, decrease the OSCCAL
        }
        else if (temp < 6120)
        {
            OSCCAL++;   // the internRC oscillator runs to slow, increase the OSCCAL
        }
        else
            calibrate = 1;//TRUE;   // the interRC is correct
    
        TCCR1B = (1<<CS10); // start timer1
    }
}

/*****************************************************************************
*
*   Function name : Delay
*
*   Returns :       None
*
*   Parameters :    unsigned int millisec
*
*   Purpose :       Delay-loop
*
*****************************************************************************/
void Delay(unsigned int millisec)
{
    unsigned char i; // mt int i;
    
    while (millisec--)
        for (i=0; i<125; i++);
}

#endif
