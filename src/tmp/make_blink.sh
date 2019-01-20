#!/bin/bash

#rm -f *.hex *.elf *.o

avr-gcc -g -Os -std=gnu99 -mmcu=atmega328p -c blink.c
avr-gcc -g -mmcu=atmega328p -o blink.elf blink.o
avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
