#!/bin/bash

rm full.hex

head -n -1 blink.hex > full.hex
cat boot.hex >> full.hex

avrdude -c usbasp -p atmega328p -U flash:w:full.hex
