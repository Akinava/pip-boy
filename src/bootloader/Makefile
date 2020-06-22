#=======================================

# Name of the project
TARGET = boot
APP_NAME = SDBOOT

# tipe of MK
MCU  = atmega328p
# cpu MK in Hz
F_CPU   = 16000000

# programmer
AVRDUDE_PROGRAMMER = usbasp

# programmer port
#PROGRAMMER_PORT = usb

BOOT = true
# 2048 bytes
BOOT_ADDRESS = 0x7800
HIGH_FUSE=0xDA
# 4096 bytes
#BOOT_ADDRESS = 0x7000
#HIGH_FUSE=0xD8

LOW_FUSE = 0xFF
EXTENDED_FUSE = 0xFD

#=======================================

OBJECTS = $(TARGET).o

# Define all object files.                                                      
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

#LDFLAGS = -L./OLED_I2C/

ifeq ($(BOOT),true)
	BOOTFLAG = -Wl,-Ttext=$(BOOT_ADDRESS)
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


#COMPILE = $(CC) $(CFLAGS) $(LDFLAGS) -mmcu=$(MCU)
COMPILE = $(CC) $(CFLAGS) -mmcu=$(MCU)

#============================================================================
##---------------- FUSES ----------------------------------------
FUSES = -U lfuse:w:$(LOW_FUSE):m -U hfuse:w:$(HIGH_FUSE):m -U efuse:w:$(EXTENDED_FUSE):m

##---------------- Programming Options (avrdude) ----------------

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_READ_FLASH = -U flash:r:dump_$(TARGET).hex:i

AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -c $(AVRDUDE_PROGRAMMER) $(AVRDUDE_VERBOSE)

##---------------- Programming Options (avrdude) ----------------
#============================================================================


#============================================================================
# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make all ....... to build $(TARGET).hex"
	@echo "make program ... to flash the firmware"
	@echo "make clean ..... to delete objects and hex file"
	@echo "make dump ...... to dump flash"




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


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf -C --mcu=$(MCU)
AVRMEM = avr-mem.sh $(TARGET).elf $(MCU)

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi


# Display compiler version information.
gccversion :
	@$(CC) --version


fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(FUSES)

#==>
all: begin gccversion sizebefore build sizeafter end

build: $(TARGET).hex
#------------------


#------------------
#Program the device.
program: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

#------------------
clean_msg:
	@echo
	@echo $(MSG_CLEANING)

clean_all:
	rm -f *.hex *.elf $(OBJECTS)

clean: clean_msg clean_all
#------------------


dump: $(TARGET).hex
	$(AVRDUDE) \
	$(AVRDUDE_FLAGS) \
	$(AVRDUDE_READ_FLASH)

.c.o:
	$(COMPILE) -c $< -o $@

# Обычное (generic) правило для компиляции файлов на языке ассемблера:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" не должен быть необходим, поскольку тип файлов
#  с расширением .S (здесь заглавная S) задан по умолчанию. Однако, символы
#  в верхнем регистре не всегда сохраняются в именах на Windows. Чтобы обеспечить
#  совместимость с WinAVR задайте тип файла вручную.

# Generic правило для компилирования файлов языка C в ассемблер, успользуется только для отладки.
.c.s:
	$(COMPILE) -S $< -o $@


$(TARGET).elf: $(OBJECTS)
	$(COMPILE) $(BOOTFLAG) -o $(TARGET).elf $(OBJECTS)

$(TARGET).hex: $(TARGET).elf
	rm -f $(TARGET).hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
#	avr-objcopy --debugging -O coff-ext-avr $< $(TARGET).cof
	avr-size $(TARGET).hex
	avr-size $(TARGET).elf

# цели отладки:
disasm:	$(TARGET).elf
	avr-objdump -d $(TARGET).elf

cpp:
	$(COMPILE) -E $(TARGET).c

bin: $(TARGET).hex
	$(OBJCOPY) -I ihex -O binary $(TARGET).hex ../../sd/BOOT/$(APP_NAME).BIN
