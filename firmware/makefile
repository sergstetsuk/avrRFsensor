# Name: Makefile
# Project: avr rf-sensor
# Author: Serg Stetsuk
# Creation Date: 2011.07.02
# Tabsize: 4
# Copyright: (c) 2011-2012
# License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
# This Revision: $Id: Makefile 1 2012.05.28 20:14 cs $

#~ DEVICE  = attiny85
#F_CPU   = 8000000
DEVICE  = atmega168p
F_CPU   = 18000000
#~ FUSE_L  = 0xE2
#~ FUSE_H  = 0xDE
FUSE_L  = 0xF7
FUSE_H  = 0xD7
LOCK = 0x03
AVRDUDE = avrdude -c usbasp -p $(DEVICE) # edit this line for your programmer

CFLAGS  = -Iusbdrv -I. -DDEBUG_LEVEL=0

CC = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# File names
EXEC = rfsensor.elf
HEX = $(EXEC:.elf=.hex)
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o) usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o

##############################################################################
# Fuse values for particular devices
##############################################################################
################################## ATMega8 ##################################
# ATMega8 FUSE_L (Fuse low byte):
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ BODEN (BrownOut Detector enabled)
#        +-------------------- BODLEVEL (2.7V)
# ATMega8 FUSE_H (Fuse high byte):
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (full output swing)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ WDTON (WDT not always on)
#        +-------------------- RSTDISBL (reset pin is enabled)
#
############################## ATMega48/88/168 ##############################
# ATMega*8 FUSE_L (Fuse low byte):
# 0xdf = 1 1 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ CKOUT (if 0: Clock output enabled)
#        +-------------------- CKDIV8 (if 0: divide by 8)
# ATMega*8 FUSE_H (Fuse high byte):
# 0xde = 1 1 0 1   0 1 1 0
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 0..2 (111 = disabled)
#        | | | |   + --------- EESAVE (preserve EEPROM over chip erase)
#        | | | +-------------- WDTON (if 0: watchdog always on)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ DWEN (debug wire enable)
#        +-------------------- RSTDISBL (reset pin is enabled)
#
############################## ATTiny25/45/85 ###############################
# ATTiny*5 FUSE_L (Fuse low byte):
# 0xe2 = 1 1 1 0   0 0 1 0
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (clock selection -> internal rc @8 MHz)
#        | |  +--------------- SUT 1..0 (BOD enabled, fast rising power)
#        | +------------------ CKOUT (clock output on CKOUT pin -> disabled)
#        +-------------------- CKDIV8 (divide clock by 8 -> don't divide)
# ATTiny*5 FUSE_H (Fuse high byte):
# 0xce = 1 1 0 0   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 2..0 (brownout trigger level -> 1.8V)
#        | | | |   +---------- EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ DWEN (debug wire enable)
#        +-------------------- RSTDISBL (disable external reset -> enabled)
# ATTiny*5 LOCK (Fuse high byte):
# 0xdd = 0 0 0 0   0 0 1 1
#                      ^ ^
#                      +-+---- LOCK BITS 00 - memory write and verification disabled
#                                        01 - memory write disabled
#                                        11 - no memory lock features used
#
################################ ATTiny2313 #################################
# ATTiny2313 FUSE_L (Fuse low byte):
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (clock selection -> crystal @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD enabled, fast rising power)
#        | +------------------ CKOUT (clock output on CKOUT pin -> disabled)
#        +-------------------- CKDIV8 (divide clock by 8 -> don't divide)
# ATTiny2313 FUSE_H (Fuse high byte):
# 0xdb = 1 1 0 1   1 0 1 1
#        ^ ^ ^ ^   \-+-/ ^
#        | | | |     |   +---- RSTDISBL (disable external reset -> enabled)
#        | | | |     +-------- BODLEVEL 2..0 (brownout trigger level -> 2.7V)
#        | | | +-------------- WDTON (watchdog timer always on -> disable)
#        | | +---------------- SPIEN (enable serial programming -> enabled)
#        | +------------------ EESAVE (preserve EEPROM on Chip Erase -> not preserved)
#        +-------------------- DWEN (debug wire enable)


# symbolic targets:
?: help
help:
	@echo "This Makefile has default rule hex. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make all ....... to erase chip, flash fuses and firmware and lock bits"
	@echo "make erase ..... to erase chip"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware without erase (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"
	@echo "make help ...... to show this help screen"
	@echo "make ? ......... to show this help screen"

#all: clean hex erase fuse flash #lock
all: clean hex erase flash

hex: $(EXEC)
	avr-objcopy -j .text -j .data -O ihex $(EXEC) $(HEX)
	avr-size $(EXEC)
	$(ERR)

# rule for programming fuse bits:
erase:
	$(AVRDUDE) -B 4 -e
# rule for programming fuse bits:
fuse:
	$(ERR)
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -B 4 -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

# rule for programming lock bits:
lock:
	@[ "$(LOCK)" != "" ] || \
		{ echo "*** Edit Makefile and choose value for LOCK!"; exit 1; }
	$(AVRDUDE) -B 4 -U lock:w:$(LOCK):m

# rule for uploading firmware:
flash: $(HEX)
	$(AVRDUDE) -D -U flash:w:$(HEX):i

# rule for uploading firmware:
eeprom: $(HEX)
	$(AVRDUDE) -D -U eeprom:w:eeprom_retransmit.hexinit:i

# rule for reading flash and eeprom:
read:
	$(AVRDUDE) -B 4 -U eeprom:r:eeprom.hex:i -U flash:r:flash.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f *.elf *.hex *.o *.lst *.map $(OBJECTS)

rebuild: clean hex


# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS)  -Wl,-Map=rfsensor.map -o $(EXEC)

# To obtain object files
main.o: main.c
	$(CC) -c $< -o $@ -Wa,-ahlms=$<.lst
%.o: %.c %.h
	$(CC) -c $< -o $@ -Wa,-ahlms=$<.lst
.S.o:
	$(CC) -x assembler-with-cpp -c $< -o $@

# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.DEFAULT_GOAL := rebuild