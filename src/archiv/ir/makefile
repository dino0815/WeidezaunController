  #############################################################################
  # Uses on command line:
  # make all = Make software.
  # make clean = Clean out built project files.
  # make program = Download the hex file to the device, using avrdude.
  # make clean; make all = To rebuild the project
  #############################################################################
  MCU   = atmega8   #  Mikrocontroller
  F_CPU = 4000000    # Main Oscillator Frequency

  # Target file name (without extension).
  TARGET = main
  SRC = $(TARGET).c rc5.c # Quellcode Datein

  AVRDUDE_PROGRAMMER = USBasp 
  AVRDUDE_PORT = /dev/ttyUSB0  # programmer connected to USB

  FORMAT = ihex  # Output format. (can be srec, ihex, binary)
  AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
  AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

  OPT = s  # Optimization level, can be [0, 1, 2, 3, s]
  CSTANDARD = -std=gnu99

  CFLAGS = -O$(OPT)
  CFLAGS += -Wall -Wstrict-prototypes
  CFLAGS += -DF_CPU=$(F_CPU)

  ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
  ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

  # Define programs and commands.
  SHELL = sh
  CC = avr-gcc
  OBJCOPY = avr-objcopy
  OBJDUMP = avr-objdump
  SIZE = avr-size
  NM = avr-nm
  AVRDUDE = avrdude
  REMOVE = rm -f
  COPY = cp

  OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 
  LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)

  HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex

#  all: hex elf obj

#  obj: $(OBJ)
#  elf: $(TARGET).elf
  hex: $(TARGET).hex

  program: $(TARGET).hex
	  $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

  clean: 
	  $(REMOVE) $(TARGET).hex
	  $(REMOVE) $(TARGET).elf
	  $(REMOVE) $(OBJ)
	  $(REMOVE) $(LST)

  %.elf: $(OBJ)
	  $(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

  %.hex: %.elf
	  $(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

  %.o : %.c
	  $(CC) -c $(ALL_CFLAGS) $< -o $@ 
