This directory contains the AVRStudio project file SmileyFlashTest.aps.
And the three hex files that result from selecting the device in SmileyFlashTest.h:
SmileyFlashTestArduino.hex
SmileyFlashTestBeAVR40.hex
SmileyFlashTestButterfly.hex


// Note that Butterfly USART runs at 19200 Baud
// while the Arduino and BeAVR40 run at 57600 Baud


You can choose which file to generate by following the comment in the header 
repeated here with the Butterfly selected:


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
#define Butterfly // ATmega169
//#define Arduino // ATmega328
//#define BeAVR40 // ATmega644