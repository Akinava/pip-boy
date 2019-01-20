#!/bin/bash

#rm *.o *.elf *.hex

avr-gcc -g -Os -std=gnu99 -mmcu=atmega328p -c boot.c
avr-gcc -g -mmcu=atmega328p -Wl,-Ttext=0x7e00 -o boot.elf boot.o
avr-objcopy -j .text -j .data -O ihex boot.elf boot.hex
