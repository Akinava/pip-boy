#=======================================

TARGET_PROGRAM = program
TARGET_BOOT = boot
TARGET_FRIMWARE = firmware

# tipe of MK
MCU  = atmega328p
# cpu MK in Hz
F_CPU   = 16000000

# programmer
AVRDUDE_PROGRAMMER = usbasp

# programmer port
#PROGRAMMER_PORT = usb

BOOT_ADDRESS = 0x7c00
SUBROUTINE_ADDRESS = 0x1000

#=======================================

OBJECTS = $(TARGET).o

#============================================================================

# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL


# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

OPT = s

#---------------- Compiler Options C ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
#CFLAGS += -mint8
#CFLAGS += -mshort-calls
#CFLAGS += -funsigned-char
#CFLAGS += -funsigned-bitfields
#CFLAGS += -fpack-struct
#CFLAGS += -fshort-enums
#CFLAGS += -fno-unit-at-a-time
CFLAGS += -Wall
#CFLAGS += -Wstrict-prototypes
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
#CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
#CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
#CFLAGS  += -I$(USB_DRV_DIR) -I. -DDEBUG_LEVEL=0


ifeq ($(BOOT),true)
	#BOOTFLAG = -Wl,-Ttext=$(BOOT_ADDRESS)
	BOOTFLAG = -Wl,-section-start=.blink=0x1000
endif


#============================================================================


# Define programs and commands.
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp


COMPILE = $(CC) $(CFLAGS) -mmcu=$(MCU)


#============================================================================
##---------------- Programming Options (avrdude) ----------------

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_READ_FLASH = -U flash:r:dump_$(TARGET).hex:i

AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) \
		-c $(AVRDUDE_PROGRAMMER) \
		$(AVRDUDE_VERBOSE) \

##---------------- Programming Options (avrdude) ----------------
#============================================================================


#============================================================================
# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"


#------------------------------------------------------------------
# ===== Output messages

# Define Messages
# Русский
MSG_ERRORS_NONE         = Ошибок: нет
MSG_BEGIN               = -------- Начало компиляции --------
MSG_END                 = -------- Конец компиляции ---------
MSG_SIZE_BEFORE 	= --- Размер до:
MSG_SIZE_AFTER 		= --- Размер после:
MSG_COFF                = --- Конвертирование в AVR COFF:
MSG_EXTENDED_COFF 	= --- Конвертирование в AVR Extended COFF:
MSG_FLASH               = --- Создание загрузочного файла для Flash:
MSG_EEPROM              = --- Создание загрузочного файла для EEPROM:
MSG_EXTENDED_LISTING    = --- Создание разширенного листинга:
MSG_SYMBOL_TABLE        = --- Создание таблицы обозначений:
MSG_LINKING             = --- Линковка:
MSG_COMPILING           = --- Компиляция C:
MSG_COMPILING_CPP       = --- Компиляция C++:
MSG_ASSEMBLING          = --- Ассемблирование:
MSG_CLEANING            = --- Отчистка проекта:
MSG_CREATING_LIBRARY    = --- Создание библиотеки:


#------------------
# Build the project

begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo


#------------------

$(TARGET_FRIMWARE).o:
	avr-gcc -g -DF_CPU=16000000UL -Os -std=gnu99 -mmcu=atmega328p -c $(TARGET_PROGRAM).c -o $(TARGET_PROGRAM).o
	avr-gcc -g -DF_CPU=16000000UL -Os -std=gnu99 -mmcu=atmega328p -c $(TARGET_BOOT).c -o $(TARGET_BOOT).o


$(TARGET_FRIMWARE).elf: $(TARGET_FRIMWARE).o
	avr-gcc -g -DF_CPU=16000000UL -Os -std=gnu99 -mmcu=atmega328p $(TARGET_PROGRAM).o -o $(TARGET_PROGRAM).elf
	avr-gcc -g -DF_CPU=16000000UL -Os -std=gnu99 -mmcu=atmega328p -Wl,-Ttext=0x7c00 $(TARGET_BOOT).o -o $(TARGET_BOOT).elf



$(TARGET_FRIMWARE).hex: $(TARGET_FRIMWARE).elf
	avr-objcopy -j .text -j .data -O ihex $(TARGET_PROGRAM).elf $(TARGET_PROGRAM).hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET_BOOT).elf $(TARGET_BOOT).hex


build_firmware: $(TARGET_FRIMWARE).hex
	head -n -1 $(TARGET_PROGRAM).hex > firmware.hex
	cat $(TARGET_BOOT).hex >> firmware.hex


all: begin build_firmware end


program: build_firmware
	avrdude -c usbasp -p atmega328p -U flash:w:$(TARGET_FRIMWARE).hex


dump:
	avrdude -c usbasp -p atmega328p -v -U eeprom:r:dump_eeprom.hex:i       


clean_msg:
	@echo
	@echo $(MSG_CLEANING)


clean_all:
	rm -f *.hex *.elf *.o


clean: clean_msg clean_all
