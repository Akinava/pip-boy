#=======================================

TARGET = bootloader

# tipe of MK
MCU  = atmega328p
# cpu MK in Hz
F_CPU   = 16000000

# programmer
AVRDUDE_PROGRAMMER = usbasp

# programmer port
#PROGRAMMER_PORT = usb

BOOT_ADDRESS = 0x7c00

#=======================================

OBJ = $(TARGET).o
ELF = $(TARGET).elf
HEX = $(TARGET).hex

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


BOOTFLAG = -Wl,-Ttext=$(BOOT_ADDRESS)


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

$(OBJ):
	$(COMPILE) -c $(TARGET).c -o $(OBJ)


$(ELF): $(OBJ)
	$(COMPILE) $(BOOTFLAG) $(OBJ) -o $(ELF)



$(HEX): $(ELF)
	$(OBJCOPY) -j .text -j .data -O ihex $(ELF) $(HEX)


all: begin $(HEX) end


program: all
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


dump:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_FLASH)


clean_msg:
	@echo
	@echo $(MSG_CLEANING)


clean_all:
	rm -f *.hex *.elf *.o


clean: clean_msg clean_all
