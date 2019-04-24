###############################################################################
#                                                                             #
# MAKEFILE for AVRExtint - A frequency counter using the extint pins          #
#                                                                             #
# Guy Wilson (c) 2019                                                         #
#                                                                             #
###############################################################################

PROJNAME=avrext

# Target device
DEVICE=attiny85

# Target architecture size
ARCHSIZE=8

# What is our target
ELF=$(PROJNAME).elf
TARGET=$(PROJNAME).hex

# Build output directory
BUILD=build

# Source directory
SRC=src

# Scheduler source directory
SCHEDSRC=src/sched

# Port we use to upload the target to the device
UPLOADPORT=/dev/cu.wchusbserial1410
#UPLOADPORT=/dev/cu.usbmodem1411

# C compiler
CC=avr-gcc

# Linker
LINKER=avr-gcc

# Hex file creator
OBJCOPY=avr-objcopy

# Assembly dump tool
OBJDUMP=avr-objdump

# Size tool
SIZETOOL=avr-size

# Upload tool
UPLOADTOOL=./upload.sh

# C compiler flags
CFLAGS=-c -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(DEVICE) -DF_CPU=16000000L -DARDUINO=10804 -DARDUINO_ARCH_AVR -DARCH_SIZE=$(ARCHSIZE)

# Linker flags
LFLAGS=-fuse-linker-plugin -Wl,--gc-sections -mmcu=$(DEVICE)

# Hex file creator flags
OBJCOPYFLAGS=-O ihex -R .eeprom

#Obj dump flags
OBJDUMPFLAGS=-I $(SRC) -f -s -l -S

# Size tool flags
SFLAGS=-C --mcu=$(DEVICE)

# Scheduler object files
SCHEDOBJ=$(BUILD)/scheduler.o

# Object files
OBJFILES=$(SCHEDOBJ) $(BUILD)/led_utils.o $(BUILD)/heartbeat.o $(BUILD)/rtc_attiny85.o $(BUILD)/pcint_attiny85.o $(BUILD)/error_attiny85.o $(BUILD)/spis_attiny85.o $(BUILD)/main.o

# Target
all: $(TARGET)

###############################################################################
#
# Scheduler files
#
###############################################################################
$(BUILD)/scheduler.o: $(SCHEDSRC)/scheduler.c $(SCHEDSRC)/scheduler.h $(SCHEDSRC)/schederr.h
	$(CC) $(CFLAGS) -o $(BUILD)/scheduler.o $(SCHEDSRC)/scheduler.c

###############################################################################
#
# Project files
#
###############################################################################
$(BUILD)/led_utils.o: $(SRC)/led_utils.c $(SRC)/led_utils.h
	$(CC) $(CFLAGS) -o $(BUILD)/led_utils.o $(SRC)/led_utils.c

$(BUILD)/heartbeat.o: $(SRC)/heartbeat.c $(SRC)/heartbeat.h $(SRC)/led_utils.h $(SCHEDSRC)/scheduler.h $(SRC)/taskdef.h
	$(CC) $(CFLAGS) -o $(BUILD)/heartbeat.o $(SRC)/heartbeat.c

$(BUILD)/rtc_attiny85.o: $(SRC)/rtc_attiny85.c $(SRC)/rtc_attiny85.h
	$(CC) $(CFLAGS) -o $(BUILD)/rtc_attiny85.o $(SRC)/rtc_attiny85.c

$(BUILD)/pcint_attiny85.o: $(SRC)/pcint_attiny85.c $(SRC)/pcint_attiny85.h $(SRC)/rtc_attiny85.h
	$(CC) $(CFLAGS) -o $(BUILD)/pcint_attiny85.o $(SRC)/pcint_attiny85.c

$(BUILD)/spis_attiny85.o: $(SRC)/spis_attiny85.c $(SRC)/spis_attiny85.h $(SRC)/rtc_attiny85.h $(SRC)/pcint_attiny85.h $(SCHEDSRC)/scheduler.h
	$(CC) $(CFLAGS) -o $(BUILD)/spis_attiny85.o $(SRC)/spis_attiny85.c

$(BUILD)/error_attiny85.o: $(SRC)/error_attiny85.c $(SCHEDSRC)/schederr.h $(SRC)/led_utils.h
	$(CC) $(CFLAGS) -o $(BUILD)/error_attiny85.o $(SRC)/error_attiny85.c

$(BUILD)/main.o: $(SRC)/main.c $(SCHEDSRC)/scheduler.h $(SRC)/heartbeat.h $(SRC)/taskdef.h $(SRC)/led_utils.h $(SRC)/rtc_attiny85.h $(SRC)/pcint_attiny85.h $(SCHEDSRC)/schederr.h
	$(CC) $(CFLAGS) -o $(BUILD)/main.o $(SRC)/main.c

###############################################################################
#
# Link it all together into an ELF format file
#
###############################################################################
$(BUILD)/$(ELF): $(OBJFILES)
	$(LINKER) $(LFLAGS) -o $(BUILD)/$(ELF) $(OBJFILES) -lm
	$(OBJDUMP) $(OBJDUMPFLAGS) $(BUILD)/$(ELF) > $(PROJNAME).s

###############################################################################
#
# Create the target
#
###############################################################################
$(TARGET): $(BUILD)/$(ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(BUILD)/$(ELF) $(TARGET)
	$(SIZETOOL) $(SFLAGS) $(BUILD)/$(ELF)

###############################################################################
#
# Upload to the device, use 'make install' to envoke
#
###############################################################################
install: $(TARGET)
	$(UPLOADTOOL) $(DEVICE) $(TARGET) $(UPLOADPORT)
	
clean: 
	rm $(BUILD)/*
	rm $(PROJNAME).s
	rm $(TARGET)
